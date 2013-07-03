
#include "device.h"

void test_r(){
   const char * file_name = "r_test_file";
   fat_dev_t fdv_f;
   fat_dev_init_r(&fdv_f, file_name);

   char buf[128] = {0,};
   fat_dev_read(&fdv_f, 0, buf, sizeof(buf) - 1);
   FAT_PRINT("%s\n", buf);
   FAT_PRINT("%c %c %c\n", buf[79], buf[80], buf[81]);
   fat_dev_destroy(&fdv_f);
}

void test_w(){
   const char * file_name = "w_test_file";

   char buf[6] = {0x64, 0x12, 0x24, };
   fat_dev_t fdv_f;
   fat_dev_create_file(&fdv_f, file_name);
   fat_dev_write(&fdv_f, 0, buf, 3);

   FAT_PRINT("%02x %02x %02x\n", buf[3], buf[4], buf[5]);
   fat_dev_read(&fdv_f, 0, buf + 3, 3);
   FAT_PRINT("%02x %02x %02x\n", buf[3], buf[4], buf[5]);
   fat_dev_destroy(&fdv_f);
}

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
    FAT_PRINT("test read...\n");
    test_r();

    FAT_PRINT("test write...\n");
    test_w();

    FAT_PRINT("test read&write...\n");
    test_rw();
}
