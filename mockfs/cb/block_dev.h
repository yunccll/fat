#ifndef  BLOCK_DEV_H
#define  BLOCK_DEV_H

#include "lock.h"

struct super_block;
struct file_system_type;
struct inode;

struct mcfs_dev;

struct block_device {
    char dev_name[32];
    int mode;
    unsigned int bd_block_size;
    struct super_block * bd_super;
    struct mutex bd_fsfreeze_mutex;
    int bd_fsfreeze_count;

    //inode no-use
    struct inode *      bd_inode;
    dev_t bd_dev;

    //TODO: blkdev_get ????? --> init_bd_inode
    struct mcfs_dev * bd_real_dev;
};


struct block_device * blkdev_get(const char * dev_name, int mode, struct file_system_type * type);
void blkdev_put(struct block_device * bdev, int mode);

unsigned int block_size(struct block_device *bdev);
int set_blocksize(struct block_device *bdev, int size);

int sb_set_blocksize(struct super_block *sb, int size);

int sb_min_blocksize(struct super_block *sb, int size);



void * getRealDevice(struct block_device * bdev);

#endif   /* BLOCK_DEV_H */
