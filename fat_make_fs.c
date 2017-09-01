#include "fat_make_fs.h"
#include "boot.h"
#include "fat.h"


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

int fat_make_fs(fat_dev_t * pdev){
    //1. write boot sector
    fat_boot_t boot;
    fat_boot_init(&boot);
    int ret = write_boot_sector(pdev, &boot);
    if(ret != 0) return -1;
    
    //2. write the fat
    fat_fat_t fat;
    ret = fat_fat_init(&fat, 1* two_bytes_to_short(boot.sector_size) , boot.fats, boot.fat_length, two_bytes_to_short(boot.sector_size));
    if(ret != 0) return -1;
    ret = write_fat(pdev, &fat);
    if(ret != 0) return -1;

    //3. write the root_directory
    //4. write the left data block
    
    
    fat_fat_destroy(&fat);

    fat_boot_destroy(&boot);
    return 0;
}


