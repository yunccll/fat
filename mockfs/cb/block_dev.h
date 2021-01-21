#ifndef  BLOCK_DEV_H
#define  BLOCK_DEV_H

#include "lock.h"

struct super_block;
struct file_system_type;
struct inode;
struct block_device {
    char dev_name[32];
    int mode;
    unsigned bd_block_size;
    struct super_block * bd_super;
    struct mutex bd_fsfreeze_mutex;
    int bd_fsfreeze_count;

    //TODO: blkdev_get ????? --> init_bd_inode
    struct inode *      bd_inode;
};

unsigned int blksize_bits(unsigned int size);
unsigned int block_size(struct block_device *bdev);

struct block_device * blkdev_get(const char * dev_name, int mode, struct file_system_type * type);
void blkdev_put(struct block_device * bdev, int mode);

int set_blocksize(struct block_device *bdev, int size);
int sb_set_blocksize(struct super_block *sb, int size);

int sb_min_blocksize(struct super_block *sb, int size);

#endif   /* BLOCK_DEV_H */
