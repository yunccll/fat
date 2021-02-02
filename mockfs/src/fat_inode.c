#include "fat_inode.h"
#include "mock/linux/printk.h"
#include "mock/linux/slab.h"
#include "mock/asm-generic/page.h"
#include "inode.h"
#include "super.h"

void fat_inode_free(struct fat_inode * fat_inode)
{
    kfree(fat_inode);
}
struct fat_inode * fat_inode_create()
{
    return (struct fat_inode *)kzalloc(sizeof(struct fat_inode), GFP_KERNEL);
}


struct inode * fat_alloc_inode(struct super_block * sb)
{
    struct inode * inode;
    struct fat_inode * fat_inode;
    pr_debug("DEBUG: fat_alloc_inode\n");

    fat_inode = fat_inode_create();
    if(!fat_inode) {
        return NULL;
    }

    inode = inode_of_fat_inode(fat_inode);
    if(inode){
        inode_init_always(sb, inode);
    }
    return inode;
}


