
#ifndef  FAT_SUPER_H
#define  FAT_SUPER_H

#include "types.h"
#include "super.h"

struct fat_super_block {
    int value;
};

static inline void fat_sb_save(struct super_block * sb, struct fat_super_block * sbf)
{
    if(sbf)
        sb->s_fs_info = sbf;
}
static inline struct fat_super_block * fat_sb(struct super_block * sb){
    return (struct fat_super_block*)sb->s_fs_info;
}


void fat_super_block_free(struct fat_super_block * sbf);
struct fat_super_block * fat_super_block_create(u8 * data, unsigned long size);


#endif   /* FAT_SUPER_H */
