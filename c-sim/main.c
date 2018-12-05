#include <stdio.h>

#include "blocks.h"
void test_block(){
    fat_dev_t *  dev = fat_dev_create_file("a.img");

    fat_blocks_t  * blocks = fat_blocks_create(dev);
    fat_blocks_set_block_count(blocks, 2880);
    fat_blocks_set_block_size(blocks, 512);

    long long bytes = fat_blocks_read_all(blocks);
    printf("bytes:%lld\n", bytes);

    //fat_block_view_t * bv = fat_blocks_get(blocks, 1);


    fat_blocks_destroy(blocks);
    fat_dev_destroy(dev);

}


#include "fs.h"
void test_fs(){
    fat_fs_t * fs = fat_fs_load("a.img");
    fat_fs_print(fs, 1);
    /*  
        fat_boot_print(fs);
        fat_fat_print(fs->fat);
        fat_fat_print(fs->fat_bak);
        fat_root_direntry_print(fs->root);
        fat_data_print(fs->data);
    */
    fat_fs_destroy(fs);
}




#include "test_fat_device.h"
int main(int argc, char * argv[])
{
    printf("hello test\n");
    test_block();
    test_fat_device();
    test_fs();
	return 0;
}
