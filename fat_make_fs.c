#include "fat_make_fs.h"
#include "boot.h"
#include "fat.h"
#include "dentry.h"
#include "data.h"


/*  
procedure is :
    1. format the boot_sector
    2. create super from boot_block
    3. create fat_block and write it
    4. create root_entry_block and write it
    5. create data_block  and write it
*/


/*   the boot info  to classed 
    __u8          jmp[3];           // 0xeb 0x3c 0x90
    __u8          sys_id[8];        // system id 
    struct msdos_volume_info vi;    //  msdos volumn info
    __u8   boot_code[BOOTCODE_SIZE];//  boot code 
    __u16        boot_sign;         //  boot sign 0x AA 55 low -> high

    //disk info
    __u16         secs_track;       //  sectors per track
    __u16         heads;            //  number of heads

    //global info
    __u8          media;            //  media code (unused)
    __u8          sector_size[2];   //  bytes per logical sector
    __u8          cluster_size;     //  sectors per cluster

    __u32         hidden;           //  hidden sectors (unused)


    __u16         reserved;         //  reserved sectors
    __u8          sectors[2];       //  number of sectors
    __u32         total_sect;       //  number of sectors (if sectors == 0)

    //fat info
    __u8          fats;             //  number of FATs
    __u16         fat_length;       //  sectors per FAT
    
    //root_entries
	__u8          dir_entries[2];   //  root directory entries
*/


struct fat_super {
    int bytes_per_sector;
    //int total_sectors; //TODO: 
    //int reserved_sectors;
    //int hidden_sectors;

    int boot_offset;
    //int sectors_of_boot;
    fat_boot_t boot;

    int fat_offset;
    //int numbers_of_fat;
    //int sector_per_fat;
    fat_fat_t fat;

    int root_entries_offset;
    //int entries_per_root;
    //int bytes_per_entry;
    fat_root_entries_t root_entry;

    int data_offset;
    //int sector_of_boot;
    fat_data_t data;
};
typedef struct fat_super fat_super_t;

fat_super_t * fat_super_free(fat_super_t * super){
    if(super){
        free(super);
    }
    return NULL;
}
fat_super_t * fat_super_alloc(){
    return (fat_super_t*)calloc(sizeof(fat_super_t), 1);
}

void fat_super_deinit(fat_super_t * super){
    if(super){
        fat_data_deinit(&(super->data));
        fat_root_entries_deinit(&(super->root_entry));
        fat_fat_deinit(&(super->fat));
        fat_boot_deinit(&(super->boot));
    }
}
/* 
int fat_super_init(fat_super_t * super){
    //TODO: set super info
    super->boot_offset = 0;
    super->sectors_of_boot = 1;
    super->bytes_per_sector = two_bytes_to_short(super->boot.sector_size);

    if(fat_boot_init(&(super->boot))){
        goto err_ret;
    }
    //TODO: init the super from the boot sectors

    super->fat_offset = super->sectors_of_boot * super->bytes_per_sector;
    if(fat_fat_init(&(super->fat), super->fat_offset, super->boot.fats, super->boot.fat_length, super->bytes_per_sector)){
        goto deinit_boot;
    }

    super->root_entries_offset = super->fat_offset + super->fat.number_of_fat * super->fat.sectors_per_fat * super->bytes_per_sector;
    if(fat_root_entries_init(&(super->root_entries), super->root_entries_offset, two_bytes_to_short(super->boot.dir_entries))){
        goto deinit_fat;
    }

    super->data_offset = super->root_entries_offset + super->root_entries.number_entries_in_root * sizeof(fat_dentry_t);
    if(fat_data_init(&(super->data), super->data_offset, 2880-33, super->bytes_per_sector)){
        goto deinit_root_entries; 
    }
    return 0;

deinit_root_entries:
    fat_root_entries_deinit(&(super->root_entries));
deinit_fat:
    fat_fat_deinit(&(super->fat));
deinit_boot:
    fat_boot_deinit(&(super->boot));
err_ret:
    return -1;
}
*/

/*  TODO 
int fat_super_mkfs(fat_super_t * super, fat_dev_t * pdev);
*/


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
    fat_root_entries_deinit(&root_entries);
final_fat: 
    fat_fat_deinit(&fat);
final_boot:
    fat_boot_deinit(&boot);
    return ret;
}


