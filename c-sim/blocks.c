#include "blocks.h"
#include "util.h"
#include "config.h"

struct fat_blocks_s {
    int _block_size;
    long long _block_count;
    char * _data;

    fat_dev_t * _dev;
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
        if(ptr->_data) {
            free(ptr->_data);
            ptr->_data = NULL;
        }
        ptr->_block_count = 0;
        ptr->_block_size = 0;
        ptr->_dev = NULL;
        free(ptr);
    }
}

void fat_blocks_set_block_count(fat_blocks_t * ptr, long long block_count){
    ptr->_block_count = block_count;
}
long long fat_blocks_get_block_count(fat_blocks_t * ptr){
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

int  fat_blocks_read_all(fat_blocks_t * ptr){
    long long bytes = ptr->_block_count * ptr->_block_size;
    //TRACE_PRINT("calc bytes: %llu\n", bytes);
    if(bytes > 0){
        __malloc_data(ptr, bytes);
        int ret = fat_dev_read(ptr->_dev, 0, ptr->_data, bytes);
        IF_TRUE_RETURN(ret < 0, -1, "read failed\n");
        return ret == bytes ? 0 : -1;
    }
    return 0;
}

void fat_blocks_print(fat_blocks_t * ptr){
    TRACE_PRINT("fat_blocks_t : block_size:%d, block_count:%lld, data:%p\n", ptr->_block_size, ptr->_block_count, ptr->_data);
}


fat_block_view_t * fat_block_view_create(long long block_index, char * offset, int block_size){
    fat_block_view_t * ptr = (fat_block_view_t*)calloc(sizeof(fat_block_view_t), 1);
    ptr->_index = block_index;
    ptr->_offset = offset;
    ptr->_size = block_size;
    return ptr;
}
void fat_block_view_free(fat_block_view_t * ptr){
    if(ptr){
        free(ptr);
    }
}


fat_block_view_t * fat_blocks_get(fat_blocks_t * ptr, long long block_index){
    if(ptr->_data && block_index >= 0){
        return fat_block_view_create(block_index, ptr->_data + block_index * ptr->_block_size, ptr->_block_size);
    }
    ERROR_PRINT("data not read from dev; or block_index[%lld] < 0\n", block_index);
    return NULL;
}


int fat_blocks_fill_block_view(fat_blocks_t * ptr, long long block_index, fat_block_view_t * bv){
    if(ptr->_data && block_index >= 0 && bv){
        bv->_index = block_index;
        bv->_offset = ptr->_data + block_index * ptr->_block_size;
        bv->_size = ptr->_block_size;
        return 0;
    }
    ERROR_PRINT("data not read from dev; or block_index[%lld] < 0\n", block_index);
    return -1;
}
void fat_block_view_print(fat_block_view_t * ptr){
    DEBUG_PRINT("block index: %d ", ptr->_index);
    DEBUG_PRINT("block offset: %p ", ptr->_offset);
    DEBUG_PRINT("block_size: %d ", ptr->_size);
    DEBUG_PRINT("\n");
}
