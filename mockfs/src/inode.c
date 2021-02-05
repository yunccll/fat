#include "inode.h"

#include "mock/linux/lock.h"
#include "mock/linux/list.h"
#include "mock/linux/printk.h"
#include "mock/linux/slab.h"
#include "mock/asm-generic/bug.h"


#include "super.h"


struct inode * inode_create()
{
    return (struct inode*)kzalloc(sizeof(struct inode), GFP_KERNEL);
}
static void inode_free(struct inode * inode)
{
	if(inode)
		kfree(inode);
}



static void inode_sb_list_add(struct inode *inode)
{
    spin_lock(&inode->i_sb->s_inode_list_lock);
    list_add(&inode->i_sb_list, &inode->i_sb->s_inodes);
    spin_unlock(&inode->i_sb->s_inode_list_lock);
}

int inode_init_always(struct super_block * sb, struct inode * inode)
{
	inode->i_sb = sb;
	inode->i_blkbits = sb->s_blocksize_bits;
    inode->i_flags = 0;

    inode->i_ino = 0;
    inode->i_size = 0;
    inode->i_blocks = 0;
    inode->i_bytes = 0;

	spin_lock_init(&inode->i_lock);
    i_uid_write(inode, 0);
    i_gid_write(inode, 0);

    init_rwsem(&inode->i_rwsem);
    atomic64_set(&inode->i_version, 0);
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

void drop_nlink(struct inode *inode)
{
	WARN_ON(inode->i_nlink == 0);
	inode->__i_nlink--;
	if (!inode->i_nlink)
		atomic64_inc(&inode->i_sb->s_remove_count);
}

/**
 * clear_nlink - directly zero an inode's link count
 * @inode: inode
 *
 * This is a low-level filesystem helper to replace any
 * direct filesystem manipulation of i_nlink.  See
 * drop_nlink() for why we care about i_nlink hitting zero.
 */
void clear_nlink(struct inode *inode)
{
	if (inode->i_nlink) {
		inode->__i_nlink = 0;
		atomic64_inc(&inode->i_sb->s_remove_count);
	}
}

/**
 * set_nlink - directly set an inode's link count
 * @inode: inode
 * @nlink: new nlink (should be non-zero)
 *
 * This is a low-level filesystem helper to replace any
 * direct filesystem manipulation of i_nlink.
 */
void set_nlink(struct inode *inode, unsigned int nlink)
{
	if (!nlink) {
		clear_nlink(inode);
	} else {
		/* Yes, some filesystems do change nlink from zero to one */
		if (inode->i_nlink == 0)
			atomic64_dec(&inode->i_sb->s_remove_count);

		inode->__i_nlink = nlink;
	}
}

/**
 * inc_nlink - directly increment an inode's link count
 * @inode: inode
 *
 * This is a low-level filesystem helper to replace any
 * direct filesystem manipulation of i_nlink.  Currently,
 * it is only here for parity with dec_nlink().
 */
void inc_nlink(struct inode *inode)
{
	if (unlikely(inode->i_nlink == 0)) {
		WARN_ON(!(inode->i_state & I_LINKABLE));
		atomic64_dec(&inode->i_sb->s_remove_count);
	}

	inode->__i_nlink++;
}
