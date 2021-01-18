
#ifndef  INODE_H
#define  INODE_H

#include "list.h"

struct super_block;

struct inode {
    struct list_head    i_sb_list;  //inode-list on super_block
    struct super_block * i_sb;
    unsigned long i_state;
};

void iput(struct inode * inode);

struct inode * new_inode(struct super_block * sb);



#endif   /* INODE_H */
