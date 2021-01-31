#include "dcache.h"
#include "mock/linux/printk.h"
#include "mock/linux/slab.h"

#include "inode.h"

static struct dentry *d_alloc_anon(struct super_block *sb)
{
    //return __d_alloc(sb, NULL);
    struct dentry * dentry ;
    pr_debug("d_alloc_anon\n");

    dentry = (struct dentry *)kzalloc(sizeof(struct dentry), GFP_KERNEL);
    if(dentry){
        dentry->d_sb = sb;
        dentry->d_iname[DNAME_INLINE_LEN-1] = 0;

        //TODO:d_set_d_op(dentry, sb->s_d_op);
        //....
    }
    return dentry;
}


static void d_instantiate(struct dentry * res, struct inode *inode)
{
    //TODO:    
}



struct dentry * d_make_root(struct inode * root_inode)
{
    struct dentry * res = 0;
    pr_debug("d_make_root\n");
    if(root_inode){
        res = d_alloc_anon(root_inode->i_sb); 
        if(res)
            d_instantiate(res, root_inode);
        else
            iput(root_inode); //TODO:
    }
    return res;
}
