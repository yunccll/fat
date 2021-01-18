#ifndef MOCK_H
#define MOCK_H


#define DNAME_INLINE_LEN	128
struct qstr {
	//TODO:
};

struct super_block;

struct dentry_operations {
};
struct dentry {
	unsigned int d_flags;		/* protected by d_lock */

	struct dentry *d_parent;	/* parent directory */
	struct qstr d_name;
	struct inode *d_inode;		/* Where the name belongs to - NULL is negative */
	unsigned char d_iname[DNAME_INLINE_LEN];	/* small names */
	const struct dentry_operations *d_op;

	struct super_block *d_sb;	/* The root of the dentry tree */

} __randomize_layout;



#endif 

