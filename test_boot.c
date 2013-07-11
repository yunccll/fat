#include "test_boot.h"

#include "device.h"

static void print_boot()
{
    
    const char * file_name = "a.img.flp";

    fat_boot_t boot;
    int ret = fat_boot_read(&boot, file_name, 0);
    fat_boot_print(&boot);
     
}


void test_boot()
{
    print_boot(); 
}

