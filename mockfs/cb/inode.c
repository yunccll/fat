#include "inode.h"


static struct inode * alloc_inode(struct super_block * sb)
{
    //TODO: need to copy from  kernel
    struct inode  * inode = (struct inode*)calloc(sizeof(struct inode), 1);
    if(inode){
        //init it
        inode->i_sb = sb;
    }
    return inode;
}

static struct inode * new_inode_pseudo(struct super_block * sb)
{
    struct inode *inode = alloc_inode(sb);

    if (inode) {
        //spin_lock(&inode->i_lock);
        inode->i_state = 0;
        //spin_unlock(&inode->i_lock);
        INIT_LIST_HEAD(&inode->i_sb_list);
    }
    return inode;
}
void inode_sb_list_add(struct inode *inode)
{
    //spin_lock(&inode->i_sb->s_inode_list_lock);
    list_add(&inode->i_sb_list, &inode->i_sb->s_inodes);
    //spin_unlock(&inode->i_sb->s_inode_list_lock);
}

struct inode * new_inode(struct super_block * sb)
{
	struct inode *inode;

    //spin_lock_prefetch(&sb->s_inode_list_lock);

    inode = new_inode_pseudo(sb);
    if (inode)
        inode_sb_list_add(inode);
    return inode;  
}

void iput(struct inode * inode)
{
    //TODO:
}

