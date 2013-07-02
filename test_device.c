
#include "device.h"

void test_rw()
{
    // set_up();
    const char * f_file = "a.img.flp";

    const char * t_file = "b.img.flp";
    fat_dev_t fdv_f;
    fat_dev_init_rw(&fdv_f, f_file);

    //read a file with content
    char  buf[2048] = {0,};
    fat_dev_read(&fdv_f, 0, buf, sizeof(buf));
    FAT_PRINT("jmp: %02x %02x %02x\n", buf[0], buf[1], buf[2]);
    fat_dev_destroy(&fdv_f);

    //write to another file
    fat_dev_t fdv_t;
    fat_dev_create_file(&fdv_t, t_file);
    fat_dev_write(&fdv_t, 0, buf, sizeof(buf));
    FAT_PRINT("jmp: %02x %02x %02x\n", buf[0], buf[1], buf[2]);

    fat_dev_destroy(&fdv_t);
//    tear_down();
}


void test_device()
{
    //1. lll......
    test_rw();
}
