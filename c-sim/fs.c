#include "fs.h"
#include "util.h"

#include "device.h"
#include "blocks.h"
#include "config.h"
#include "boot.h"
#include "fat.h"

struct fat_fs_s {
    int _boot_block_index;
    fat_boot_t * _boot;

    int _fat_block_index;
    fat_fat_t * _fat;
    //fat_fat_t * _fat_bak;
    //fat_root_dir_t * _root;
    //fat_data_t  * data;

    fat_blocks_t * _blocks;
    fat_dev_t * _dev;
};


void fat_fs_destroy(fat_fs_t * ptr){
    if(ptr){
        //TODO: 
        fat_fat_destroy(ptr->_fat);
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

    ptr->_boot_block_index = 0;
    ptr->_boot = fat_boot_create();
    IF_TRUE_GOTO(ptr->_boot == NULL, err_free_blocks, "create boot failed\n");

    ptr->_fat_block_index = ptr->_boot_block_index + ptr->_boot->_reserved_sector_count;
    ptr->_fat = fat_fat_create();
    IF_TRUE_GOTO(ptr->_fat == NULL, err_free_boot, "create fat failed\n");
    return ptr;

err_free_boot:
    fat_fat_destroy(ptr->_fat); 
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

    fat_block_view_t * bv = fat_blocks_get(ptr->_blocks, ptr->_boot_block_index);
    IF_TRUE_RETURN(bv == NULL, -1, "get boot block error!\n");

    ret = fat_boot_parser(ptr->_boot, bv->_offset, bv->_size);
    IF_TRUE_RETURN(ret != 0, -1, "parser boot block failed\n");

    ptr->_fat_block_index = ptr->_boot_block_index + ptr->_boot->_reserved_sector_count;
    ret = fat_blocks_fill_block_view(ptr->_blocks, ptr->_fat_block_index, bv);
    IF_TRUE_RETURN(ret != 0, -1, "get fat block failed\n");
    fat_block_view_print(bv);
    ret = fat_fat_parser(ptr->_fat, bv->_offset, ptr->_boot->_sector_per_fat * bv->_size);
    IF_TRUE_RETURN(ret != 0, -1, "parser fat block failed\n");

    /* 
    //TODO: 
    ptr->_fat_bak_block_index = ptr->_fat_block_index + ptr->_boot->_sector_per_fat;
    ret = fat_blocks_fill_block_view(ptr->_blocks, ptr->_boot_block_index + ptr->_boot->_reserved_sector_count, bv)
    IF_TRUE_RETURN(ret != 0, -1, "get fat block failed\n");
    ret = fat_fat_parser(ptr->_fat, bv->_offset, bv->_size + ptr->_boot->_sector_per_fat)
    IF_TRUE_RETURN(ret != 0, -1, "parser fat block failed\n");
    */


    //parse fat_back info
    //parse root directory
    return 0;
}


int fat_fs_print(fat_fs_t * ptr){
    assert(ptr && ptr->_dev);

    fat_blocks_print(ptr->_blocks);
    fat_dev_print(ptr->_dev);

    fat_boot_print(ptr->_boot);
    return 0;
}
