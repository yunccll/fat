#include "inode.h"
#include "lock.h"
#include "list.h"
#include "super.h"


static struct inode * inode_create()
{
    return (struct inode*)calloc(sizeof(struct inode), 1);
}
static void inode_free(struct inode * inode)
{
	if(inode)
		free(inode);
}



static void inode_sb_list_add(struct inode *inode)
{
    spin_lock(&inode->i_sb->s_inode_list_lock);
    list_add(&inode->i_sb_list, &inode->i_sb->s_inodes);
    spin_unlock(&inode->i_sb->s_inode_list_lock);
}

static int inode_init_always(struct super_block * sb, struct inode * inode)
{
	inode->i_sb = sb;
	inode->i_blkbits = sb->s_blocksize_bits;
    inode->i_flags = 0;

    inode->i_ino = 0;
    inode->i_size = 0;
    inode->i_blocks = 0;
    inode->i_bytes = 0;

	spin_lock_init(&inode->i_lock);

    init_rwsem(&inode->i_rwsem);
    //TODO:
    return 0;
}

static struct inode * alloc_inode(struct super_block * sb)
{
    const struct super_operations *ops = sb->s_op;
    struct inode * inode;
    
    if(ops->alloc_inode){
        inode = ops->alloc_inode(sb); // return struct inode;
    }
    else{
        inode = inode_create();
    }

    if(!inode){
        return NULL;
    }

    if(inode_init_always(sb, inode)){
        if(ops->destroy_inode){
            ops->destroy_inode(inode);
            if(!ops->free_inode) 
                return NULL;
        }
		inode_free(inode);
//      rcu related
//      inode->free_inode = ops->free_inode；
//      i_callback(&inode->i_rcu); 
        return NULL;
    }
    return inode;
}

static struct inode * new_inode_pseudo(struct super_block * sb)
{
    struct inode *inode = alloc_inode(sb);
    if(inode){
        spin_lock(&inode->i_lock);
        inode->i_state = 0;  //init
        spin_unlock(&inode->i_lock);
        INIT_LIST_HEAD(&inode->i_sb_list); //init
    }
    return inode;
}

struct inode * new_inode(struct super_block * sb)
{
	struct inode *inode;
    pr_debug("new inode for root\n");

    //spin_lock_prefetch(&sb->s_inode_list_lock);

    inode = new_inode_pseudo(sb);
    if (inode)
        inode_sb_list_add(inode);
    return inode;  
}

void iput(struct inode * inode)
{
    pr_debug("input\n");
    //TODO:
}
