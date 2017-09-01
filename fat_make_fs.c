#include "fat_make_fs.h"
#include "boot.h"


static int write_boot_sector(fat_dev_t * pdev){
    DECLARE_FAT_BOOT(boot);
    fat_boot_format12(&boot);
    return fat_boot_write_to_dev(&boot, pdev, 0);
}

static int write_fat(fat_dev_t * pdev, fat_fat_t * pfat){
    return -1;
}

int fat_make_fs(fat_dev_t * pdev){
    //1. write boot sector
    write_boot_sector(pdev);
    //2. write the fat1
    //3. write the fat2
    //4. write the root_directory
    //4.1 write "."  for root_directory
    //4.2 write ".." for root_directory
    //5. write the left data block
    return 0;
}


