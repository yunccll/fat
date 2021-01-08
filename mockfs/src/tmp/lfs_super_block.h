
#ifndef  LFS_SUPER_BLOCK_H
#define  LFS_SUPER_BLOCK_H

#include "mock.h"

int lfs_fill_super(struct super_block * sb, void * data, int silent);

#endif   /* LFS_SUPER_BLOCK_H */
