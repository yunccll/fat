
#ifndef  BUFFER_HEAD_H
#define  BUFFER_HEAD_H

#include <stddef.h>

#include "types.h"
#include "lock.h"

struct super_block;
struct block_device;
struct buffer_head {
    atomic_t b_count;
    size_t b_size;
    sector_t b_blocknr;
    struct block_device * b_bdev;
};

//TODO: create buffer_head ?????
struct buffer_head * sb_bread(struct super_block *sb, sector_t block);
void __brelse(struct buffer_head *);
static inline void brelse(struct buffer_head *bh)
{
    if (bh)
        __brelse(bh);
}

void map_bh(struct buffer_head *bh, struct super_block *sb, sector_t block);


#endif   /* BUFFER_HEAD_H */
