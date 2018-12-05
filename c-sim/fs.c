#include "fs.h"
#include "util.h"

#include "device.h"

struct fat_fs_s {
    int _boot_block_offset;
    //fat_boot_t * _boot;
    //fat_fat_t * _fat, _fat_bak;
    //fat_root_dir_t * _root;
    //fat_data_t  * data;

    //TODD: fat_blocks_t * blocks;
    fat_dev_t * _dev;
};


void fat_fs_destroy(fat_fs_t * ptr){
    if(ptr){
        //TODO: 
        //fat_boot_destroy(ptr->_boot);
        fat_dev_destroy(ptr->_dev);
        free(ptr);
    }
}

fat_fs_t * fat_fs_load(const char * dev_name){
    assert(dev_name);

    fat_fs_t * ptr = (fat_fs_t *)calloc(sizeof(fat_fs_t), 1);
    IF_TRUE_RETURN(ptr == NULL, NULL, "no enough memory\n");   
    ptr->_boot_block_offset = 0;
    
    ptr->_dev = fat_dev_create_file(dev_name);
    IF_TRUE_RETURN(ptr->_dev == NULL, NULL, "open dev failed for dev_name:[%s]\n", dev_name);

    //TODO: 
    /*  
    fat_block_view_t * bv = fat_buffer_block_view(buffer, ptr->_boot_block_offset);
    IF_TRUE_GOTO(bv == NULL, err_free_dev, "get block:[%d] failed\n", ptr->_boot_block_offset);
    ptr->_boot = fat_boot_load(block);
    */
    return ptr;

err_free_dev:
    fat_dev_destroy(ptr->_dev);
error:
    free(ptr);
    return NULL;
}

int fat_fs_print(fat_fs_t * ptr, int fd){
    assert(ptr && ptr->_dev);
    //TODO: 
    char buf[512];
    int ret = fat_dev_read(ptr->_dev, 0, buf, sizeof(buf));

    DEBUG_PRINT("fs print to fd:%d,  fname:%s, ret:%d\n", fd, fat_dev_fname(ptr->_dev), ret);
    return 0;
}
