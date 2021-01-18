
#ifndef  DCACHE_H
#define  DCACHE_H


struct dentry {
//TODO:
	struct super_block *d_sb;	/* The root of the dentry tree */
    struct inode * d_inode;
};

static inline struct dentry *dget(struct dentry *dentry)
{
    //TODO:
    //if (dentry)
        //lockref_get(&dentry->d_lockref);
    return dentry;
}


struct dentry * d_make_root(struct inode * root_inode);



#endif   /* DCACHE_H */
