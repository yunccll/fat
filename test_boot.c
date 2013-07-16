#include "test_boot.h"

#include "device.h"

static void print_boot()
{
    FAT_PRINT("Print a real device 's boot sector\n");
    const char * file_name = "a.img.flp";
    DECLARE_FAT_BOOT(boot);
    int ret = fat_boot_read(&boot, file_name, 0);
    fat_boot_print(&boot);
    FAT_PRINT("Finished......\n\n");
}


static void init_boot()
{
    FAT_PRINT("Init boot sector and print it\n");
    DECLARE_FAT_BOOT(boot);
    fat_boot_format12(&boot);
    fat_boot_print(&boot);
    FAT_PRINT("Finished......\n\n");
}

static void other_info()
{
    FAT_PRINT("Other info print it\n");
    DECLARE_FAT_BOOT(boot);
    fat_boot_format12(&boot);
    
    PRINT_OUT("BOOT_SECTOR_SIZE     : %d\n", BOOT_SECTOR_SIZE);
    PRINT_OUT("fat1_start_sector    : %d\n", fat1_start_sector);
    PRINT_OUT("fat1_end_sector      : %d\n", fat1_end_sector(boot.fat_length));

    PRINT_OUT("fat2_start_sector    : %d\n", fat2_start_sector(boot.fat_length));
    PRINT_OUT("fat2_end_sector      : %d\n", fat2_end_sector(boot.fat_length));

    PRINT_OUT("FAT_DIR_ENTRY_SIZE   : %d\n", FAT_DIR_ENTRY_SIZE);
    PRINT_OUT("fat_root_dir_sectno  : %d\n", fat_root_dir_sectno(&boot));
    PRINT_OUT("fat_root_dir_start_sectno    : %d\n", fat_root_dir_start_sectno(&boot));
    PRINT_OUT("fat_root_dir_end_sectno      : %d\n", fat_root_dir_end_sectno(&boot));

    FAT_PRINT("Finished......\n\n");
}

void test_boot()
{
    print_boot(); 
    init_boot();
    other_info();
}

