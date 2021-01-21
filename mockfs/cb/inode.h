#ifndef  INODE_H
#define  INODE_H

#include "base.h"
#include "list.h"
#include "lock.h"
#include "time64.h"


struct super_block;

struct inode {
    struct super_block * i_sb;
    struct list_head i_sb_list;


    struct rw_semaphore i_rwsem;

    unsigned int i_flags;
    
    unsigned long i_ino;
    loff_t          i_size;
    struct timespec64   i_atime;
    struct timespec64   i_mtime;
    struct timespec64   i_ctime;

    spinlock_t i_lock;
    unsigned long i_state;
    blkcnt_t        i_blocks;
    unsigned short  i_bytes;
   	u8 i_blkbits;

//    const struct inode_operations   *i_op;
//    struct super_block  *i_sb;
//    struct address_space    *i_mapping;
};

struct inode * new_inode(struct super_block * sb);


void iput(struct inode * inode);

#endif   /* INODE_H */
