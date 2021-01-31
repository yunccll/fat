
#ifndef  FAT_INODE_H
#define  FAT_INODE_H

/* DOS dates from 1980/1/1 through 2107/12/31 */
#define FAT_DATE_MIN (0<<9 | 1<<5 | 1)
#define FAT_DATE_MAX (127<<9 | 12<<5 | 31)
#define FAT_TIME_MAX (23<<11 | 59<<5 | 29)



struct inode;
struct super_block;

struct inode * fat_alloc_inode(struct super_block * sb);


#endif   /* FAT_INODE_H */
