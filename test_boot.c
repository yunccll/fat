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

void test_boot()
{
    print_boot(); 
    init_boot();
}

