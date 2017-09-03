#include "fat_make_fs.h"
#include "boot.h"
#include "fat.h"
#include "dentry.h"
#include "data.h"


static int write_boot_sector(fat_dev_t * pdev, fat_boot_t * pboot){
    fat_boot_format12(pboot);
    return fat_boot_write_to_dev(pboot, pdev, 0) < 0 ? -1 : 0;
}

static int write_fat(fat_dev_t * pdev, fat_fat_t * pfat){
    int i = 0; 
    for(; i < pfat->number_of_fat; ++i){
        int bytes = pfat->sectors_per_fat * pfat->bytes_per_sector;
        if(fat_dev_write(pdev, pfat->start_offset + i * bytes , pfat->fats[i], bytes) < 0)  goto error;
    }
    return 0;
error:
    return -1;
}

static int write_root_entries(fat_dev_t * pdev, fat_root_entries_t * root_entries){
    return fat_dev_write(pdev, root_entries->start_offset, root_entries->entries, root_entries->number_entries_in_root * sizeof(fat_dentry_t)) < 0 ? -1 : 0;
    //TODO: add the . && .. fat_dentry
}
static int write_data_blocks(fat_dev_t * pdev, fat_data_t * pdata){
    int i = 0;
    for(; i < pdata->sectors_per_data; ++i){
        int ret = fat_dev_write(pdev, pdata->start_offset + i * pdata->bytes_per_sector,  
        pdata->data_block, pdata->bytes_per_sector);
        if(ret < 0){
            return -1;
        }
    }
    return 0;
}

#define SECTOR_OF_BOOT      1

int fat_make_fs(fat_dev_t * pdev){
    //1. write boot sector
    fat_boot_t boot;
    fat_boot_init(&boot);
    int ret = write_boot_sector(pdev, &boot);
    if(ret != 0) {
        ret = -1;
        goto final_boot;
    }
    
    //2. write the fat
    fat_fat_t fat;
    ret = fat_fat_init(&fat, SECTOR_OF_BOOT* two_bytes_to_short(boot.sector_size) , boot.fats, boot.fat_length, two_bytes_to_short(boot.sector_size));
    if(ret != 0){ 
        ret = -1;
        goto final_boot;
    }
    ret = write_fat(pdev, &fat);
    if(ret != 0){
        ret = -1;
        goto final_fat;
    }

    //3. write the root_entries
    fat_root_entries_t root_entries;
    int start_offset = fat.start_offset + fat.number_of_fat * fat.sectors_per_fat * fat.bytes_per_sector;
    ret = fat_root_entries_init(&root_entries, start_offset, two_bytes_to_short(boot.dir_entries));
    if(ret != 0){
        ret = -1;
        goto final_root_entries;
    }
    ret = write_root_entries(pdev, &root_entries);
    if(ret != 0){
        ret = -1;
        goto final_root_entries;
    }

    //4. write the left data block
    fat_data_t data;
    start_offset = root_entries.start_offset + root_entries.number_entries_in_root * sizeof(fat_dentry_t);
    ret = fat_data_init(&data, start_offset, 2880-33, fat.bytes_per_sector); //TODO: 
    if(ret != 0){
        ret = -1;
        goto final_data;
    }
    ret = write_data_blocks(pdev, &data);
    if(ret != 0){
        ret = -1;
        goto final_data;
    }
    return 0;

final_data:
    fat_data_deinit(&data);
final_root_entries:
    fat_root_entries_destroy(&root_entries);
final_fat: 
    fat_fat_destroy(&fat);
final_boot:
    fat_boot_destroy(&boot);
    return ret;
}


