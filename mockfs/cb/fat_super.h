
#ifndef  FAT_SUPER_H
#define  FAT_SUPER_H

#include "types.h"
#include "super.h"

struct fat_sb {
    int value;
};

static inline void fat_sb_save(struct super_block * sb, struct fat_sb * fsb)
{
    if(fsb)
        sb->s_fs_info = fsb;
}
static inline struct fat_sb * fat_sb(struct super_block * sb){
    return (struct fat_sb*)sb->s_fs_info;
}


void fat_sb_free(struct fat_sb * fsb);
struct fat_sb * fat_sb_create(u8 * data, unsigned long size);


#endif   /* FAT_SUPER_H */
