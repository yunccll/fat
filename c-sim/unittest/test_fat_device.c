#include "test_fat_device.h"

#include "device.h"
#include <assert.h>

static void test_device(void){
    
    fat_dev_t * ptr = fat_dev_create_file("hello.img");
    assert(ptr);

    char buf [512] = {0x11, 0x22, 0x33, 0x44};
    int ret = fat_dev_write(ptr, 0, (const uchar *)buf, sizeof(buf));
    assert(ret == sizeof(buf));


    char new_buf[512] = {0};
    ret = fat_dev_read(ptr, 0, (uchar *)new_buf, sizeof(new_buf));
    assert(ret == sizeof(new_buf));

    assert(new_buf[0] == buf[0]);
    assert(new_buf[1] == buf[1]);
    assert(new_buf[2] == buf[2]);
    assert(new_buf[3] == buf[3]);

    fat_dev_destroy(ptr);
}



void test_fat_device(void){
    test_device();
}
