#ifndef  SUPER_H
#define  SUPER_H


#include "list.h"
#include "lock.h"
#include "types.h"

struct dengry;
struct file_system_type;
struct super_operations;

    //sb->s_dev = sb->s_bdev->bd_dev;
struct super_block {
    struct list_head s_list;
    struct block_device * s_bdev;
    struct hlist_node s_instances;
    struct file_system_type * s_type;
    char s_id[32];
    int s_flags;

    dev_t s_dev;

    int         s_count; //ref count
    atomic_t    s_active;

    loff_t      s_maxbytes;
    u32         s_time_gran;
    time64_t    s_time_min;
    time64_t    s_time_max;

    const struct super_operations   *s_op;

    unsigned long   s_magic;
    struct dentry   *s_root;
    fmode_t         s_mode;

    struct rw_semaphore s_umount;/* ref count lock */
    unsigned char s_blocksize_bits;
    unsigned long s_blocksize;

    unsigned long s_iflags; /* internal SB_I_* flags  */
    void * s_fs_info;
    
    spinlock_t  s_inode_list_lock;
    struct list_head s_inodes;

    //TODO: 
    const struct dentry_operations * s_d_op; /*  default d_op for dentries */
    
    atomic_long_t s_remove_count; /* number of inode with nlink == 0 but still referenced */
};



struct dentry *mount_bdev(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data,
	int (*fill_super)(struct super_block *, void *, int));

void kill_block_super(struct super_block *sb);


struct super_operations {
	struct inode *(*alloc_inode)(struct super_block *sb);
	void (*destroy_inode)(struct inode *);
	void (*free_inode)(struct inode *);

	void (*dirty_inode) (struct inode *, int flags);
	//int (*write_inode) (struct inode *, struct writeback_control *wbc);
	int (*drop_inode) (struct inode *);
	void (*evict_inode) (struct inode *);
	void (*put_super) (struct super_block *);
	int (*sync_fs)(struct super_block *sb, int wait);
	int (*freeze_super) (struct super_block *);
	int (*freeze_fs) (struct super_block *);
	int (*thaw_super) (struct super_block *);
	int (*unfreeze_fs) (struct super_block *);
	//int (*statfs) (struct dentry *, struct kstatfs *);
	int (*remount_fs) (struct super_block *, int *, char *);
	void (*umount_begin) (struct super_block *);

	//int (*show_options)(struct seq_file *, struct dentry *);
	//int (*show_devname)(struct seq_file *, struct dentry *);
	//int (*show_path)(struct seq_file *, struct dentry *);
	//int (*show_stats)(struct seq_file *, struct dentry *);

	//int (*bdev_try_to_free_page)(struct super_block*, struct page*, gfp_t);
	//long (*nr_cached_objects)(struct super_block *, struct shrink_control *);
	//long (*free_cached_objects)(struct super_block *, struct shrink_control *);
};

// put_super
// drop_super
// iterato_super


#endif   /* SUPER_H */
