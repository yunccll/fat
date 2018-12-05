
#ifndef  BLOCKS_H
#define  BLOCKS_H

#include "device.h"

//#include "block_view.h"

typedef struct fat_blocks_s fat_blocks_t;
typedef struct fat_block_view_s fat_block_view_t;


fat_blocks_t * fat_blocks_create(fat_dev_t * dev);
void fat_blocks_destroy(fat_blocks_t * ptr);

long long fat_blocks_read_all(fat_blocks_t * ptr);

fat_block_view_t * fat_blocks_get(fat_blocks_t * ptr, fat_offset_t offset);

void fat_blocks_set_block_count(fat_blocks_t * ptr, int block_count);
int fat_blocks_get_block_count(fat_blocks_t * ptr);

void fat_blocks_set_block_size(fat_blocks_t * ptr, int block_size);
int fat_blocks_get_block_size(fat_blocks_t * ptr);




#endif   /* BLOCKS_H */
