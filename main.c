#include "boot.h"

#include "device.h"

#include "common.h"

void test_device()
{
    const char * file = "a.img.flp";
    fat_dev_t fdv;
    fat_dev_init(&fdv, file, O_RDWR);

    char  buf[2048] = {0,};
    fat_dev_read(&fdv, 0, buf, sizeof(buf));



}

int main(int argc, char * argv[])
{
    fat_boot_t b;
    fat_boot_init(&b);
    fat_boot_print(&b);

    test_device();
    return 0;
}
