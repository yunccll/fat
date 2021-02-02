
#ifndef  FAT_INODE_H
#define  FAT_INODE_H

#include "inode.h"

/* DOS dates from 1980/1/1 through 2107/12/31 */
#define FAT_DATE_MIN (0<<9 | 1<<5 | 1)
#define FAT_DATE_MAX (127<<9 | 12<<5 | 31)
#define FAT_TIME_MAX (23<<11 | 59<<5 | 29)



struct super_block;

struct fat_inode {
    unsigned long fi_start;
    struct inode fi_vfs_inode;
};

static inline struct fat_inode * fat_inode(struct inode * inode){
    return (struct fat_inode *)container_of(inode, struct fat_inode, fi_vfs_inode);
}
static inline struct inode * inode_of_fat_inode(struct fat_inode * fat_inode){
    return &(fat_inode->fi_vfs_inode);
}

struct inode * fat_alloc_inode(struct super_block * sb);


#endif   /* FAT_INODE_H */
