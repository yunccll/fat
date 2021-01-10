#ifndef  SUPER_H
#define  SUPER_H

struct dengry;
struct file_system_type;

struct super_block {
//TODO:
};


struct dentry *mount_bdev(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data,
	int (*fill_super)(struct super_block *, void *, int));

void kill_block_super(struct super_block *sb);

#endif   /* SUPER_H */
