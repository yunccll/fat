#include "fs.h"
#include "util.h"

#include "device.h"
#include "blocks.h"
#include "config.h"
#include "boot.h"

struct fat_fs_s {
    int _boot_block_index;
    fat_boot_t * _boot;

    //int _fat_block_index;
    //fat_fat_t * _fat, _fat_bak;
    //fat_root_dir_t * _root;
    //fat_data_t  * data;

    fat_blocks_t * _blocks;
    fat_dev_t * _dev;
};


void fat_fs_destroy(fat_fs_t * ptr){
    if(ptr){
        //TODO: 
        fat_boot_destroy(ptr->_boot);
        fat_blocks_destroy(ptr->_blocks);
        fat_dev_destroy(ptr->_dev);
        free(ptr);
    }
}

fat_fs_t * fat_fs_create(const char * dev_name){
    assert(dev_name);

    fat_fs_t * ptr = (fat_fs_t *)calloc(sizeof(fat_fs_t), 1);
    IF_TRUE_RETURN(ptr == NULL, NULL, "no enough memory\n");   
    ptr->_boot_block_index = 0;
    
    ptr->_dev = fat_dev_create_file(dev_name);
    IF_TRUE_RETURN(ptr->_dev == NULL, NULL, "open dev failed for dev_name:[%s]\n", dev_name);

    ptr->_blocks = fat_blocks_create(ptr->_dev);
    IF_TRUE_GOTO(ptr->_blocks == NULL, err_free_dev, "create blocks from device error\n");

    ptr->_boot = fat_boot_create();
    IF_TRUE_GOTO(ptr->_boot == NULL, err_free_blocks, "create boot failed\n");
    return ptr;

err_free_blocks:
    fat_blocks_destroy(ptr->_blocks);
    ptr->_blocks = NULL;
err_free_dev:
    fat_dev_destroy(ptr->_dev);
    ptr->_dev  = NULL;
error:
    free(ptr);
    return NULL;
}


int fat_fs_load_data(fat_fs_t * ptr){
    fat_blocks_set_block_count(ptr->_blocks, FAT_12_BLOCK_COUNT);
    
    int ret = fat_blocks_read_all(ptr->_blocks);
    IF_TRUE_RETURN(ret != 0, -1, "read all blocks failed\n");

    //TODO: 
    //parse fs info
    //parse fat info
    //parse fat_back info
    //parse root directory
    return 0;
}


int fat_fs_print(fat_fs_t * ptr){
    assert(ptr && ptr->_dev);

    fat_blocks_print(ptr->_blocks);
    fat_dev_print(ptr->_dev);
    return 0;
}
