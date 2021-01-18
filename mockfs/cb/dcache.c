#include "dcache.h"
#include "inode.h"

static struct dentry * d_alloc_anon(struct super_block * sb)
{
    //TODO:
    struct dentry * dentry = (struct dentry *)calloc(sizeof(struct dentry), 1);
    if(!dentry){
        return NULL;
    }

    dentry->d_sb = sb;
    return dentry;
}
static void d_instantiate(struct dentry * dentry,  struct inode * inode)
{
    //TODO:
    dentry->d_inode = inode;
}

struct dentry * d_make_root(struct inode * root_inode)
{
	struct dentry *res = NULL;

    if (root_inode) {
        res = d_alloc_anon(root_inode->i_sb);
        if (res)
            d_instantiate(res, root_inode);
        else
            iput(root_inode);
    }
    return res;
}
