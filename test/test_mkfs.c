#include "test_mkfs.h"
#include "device.h"
#include <assert.h>

int test_mkfs_all(){

    fat_dev_t dev;
    int ret = fat_dev_create_file(&dev, "fat.img");
    assert(ret == 0);

    ret = fat_make_fs(&dev);
    assert(ret == 0);

    fat_dev_destroy(&dev);

    return 0;
}
