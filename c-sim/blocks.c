#include "blocks.h"
#include "util.h"
#include "config.h"

struct fat_blocks_s {
    int _block_size;
    int _block_count;
    char * _data;

    fat_dev_t * _dev;
};
struct fat_block_view_s {

};


fat_blocks_t * fat_blocks_create(fat_dev_t * dev){

    fat_blocks_t * ptr  = (fat_blocks_t *)calloc(sizeof(fat_blocks_t), 1);
    IF_TRUE_RETURN(ptr == NULL, NULL, "alloc failed: no enough memory\n");
    ptr->_dev = dev;
    ptr->_block_size = DEFAULT_BLOCK_SIZE;
    ptr->_block_count = DEFAULT_BLOCK_COUNT;
    ptr->_data = NULL;
    return ptr;
}

void fat_blocks_destroy(fat_blocks_t * ptr){
    if(ptr){
        free(ptr);
    }
}

void fat_blocks_set_block_count(fat_blocks_t * ptr, int block_count){
    ptr->_block_count = block_count;
}
int fat_blocks_get_block_count(fat_blocks_t * ptr){
    return ptr->_block_count;
}

void fat_blocks_set_block_size(fat_blocks_t * ptr, int block_size){
    ptr->_block_size = block_size;
}
int fat_blocks_get_block_size(fat_blocks_t * ptr){
    return ptr->_block_size;
}

static int __malloc_data(fat_blocks_t * ptr, long long bytes){
    ptr->_data = (char*)malloc(sizeof(char) * bytes);
    IF_TRUE_RETURN(ptr->_data == NULL, -1, "alloc failed: no enough memory\n");
    return 0;
}
static void __free_data(fat_blocks_t * ptr){
    if(ptr->_data != NULL){
        free(ptr->_data);
    }
}

long long  fat_blocks_read_all(fat_blocks_t * ptr){
    long long bytes = ((long long)ptr->_block_count) * ptr->_block_size;
    if(bytes > 0){
        __malloc_data(ptr, bytes);
        //read from img
        int ret = fat_dev_read(ptr->_dev, 0, ptr->_data, bytes);
        IF_TRUE_RETURN(ret < 0, -1, "read failed\n");
        return ret;
    }
    return 0;
}


fat_block_view_t * fat_blocks_get(fat_blocks_t * ptr, fat_offset_t offset){
    return NULL;
}


