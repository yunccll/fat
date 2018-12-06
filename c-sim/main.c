#include <stdio.h>
#include <assert.h>

#include "blocks.h"
void test_block(){
    fat_dev_t *  dev = fat_dev_create_file("../a.img");  //for readonly

    fat_blocks_t  * blocks = fat_blocks_create(dev);
    fat_blocks_set_block_count(blocks, 2880);
    fat_blocks_set_block_size(blocks, 512);

    assert( 0 == fat_blocks_read_all(blocks));

    {
        fat_block_view_t * bv = fat_blocks_get(blocks, 0);
        assert(bv);
    }

    fat_blocks_destroy(blocks);
    fat_dev_destroy(dev);

}


#include "fs.h"
void test_fs(){
    fat_fs_t * fs = fat_fs_create("a.img");
    assert(fs);
    fat_fs_load_data(fs); //read data, parse data 
    fat_fs_print(fs);
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
    //test_fat_device();
    test_fs();
	return 0;
}
