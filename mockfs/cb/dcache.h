
#ifndef  DCACHE_H
#define  DCACHE_H


struct dentry {
//TODO:
	struct super_block *d_sb;	/* The root of the dentry tree */
};

static inline struct dentry *dget(struct dentry *dentry)
{
    //TODO:
    //if (dentry)
        //lockref_get(&dentry->d_lockref);
    return dentry;
}



#endif   /* DCACHE_H */
