#include "fat_inode.h"
#include "inode.h"
#include "super.h"
#include "base.h"

struct inode * fat_alloc_inode(struct super_block * sb)
{
    struct inode * inode = NULL;
    pr_debug("DEBUG: fat_alloc_inode\n");
    inode = inode_create();
    if(inode){
        inode_init_always(sb, inode);
    }
    return inode;
}


