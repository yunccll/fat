
#ifndef  DCACHE_H
#define  DCACHE_H


struct inode;

#define DNAME_INLINE_LEN 32

struct dentry {
	struct super_block *d_sb;	/* The root of the dentry tree */

    unsigned char d_iname[DNAME_INLINE_LEN];
    struct inode * d_inode;    
//TODO:
    
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
