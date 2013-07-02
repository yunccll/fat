#include "boot.h"

#include "test_device.h"


int main(int argc, char * argv[])
{
    /*  fat_boot_t b;
    fat_boot_init(&b);
    fat_boot_print(&b);*/

    test_device();
    return 0;
}
