
#ifndef  FAT_INODE_H
#define  FAT_INODE_H

struct inode;
struct super_block;

struct inode * fat_alloc_inode(struct super_block * sb);

#endif   /* FAT_INODE_H */
