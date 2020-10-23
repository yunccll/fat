#ifndef MOCK_H
#define MOCK_H

#include "linux/types.h"

namespace mock {
    struct super_block {
        int s_blocksize;
        unsigned int s_blocksize_bits;
        void * s_fs_info;
    };

    static inline unsigned int blksize_bits(unsigned int size)
    {
        unsigned int bits = 8;
        do {
            bits++;
            size >>= 1;
        } while (size > 256);
        return bits;
    }
    static inline int sb_min_blocksize(struct super_block *sb, int size){
        int minsize = 512;
        if(size < minsize){
            size = minsize;
        }
        sb->s_blocksize = size;
        sb->s_blocksize_bits = blksize_bits(size);
        return sb->s_blocksize;
    }

    struct buffer_head {
        size_t b_size;      //size of mapping
        char * b_data;      //pointer to data within the page
    };

    static inline struct buffer_head * sb_bread(struct super_block *sb, sector_t block){
        return (struct buffer_head*)calloc(1, sizeof(struct buffer_head));
    }
    static inline void brelse(struct buffer_head *bh){
        free(bh);
    }

    static inline void printk(const char * str){
    }

} // end namespace mock


using namespace mock; // Use it as global function

#endif 

