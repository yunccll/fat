#ifndef  SUPER_H
#define  SUPER_H


#include "list.h"
#include "lock.h"
#include "types.h"
#include "base.h"

struct dengry;
struct file_system_type;
struct super_operations;

struct super_block {
    struct block_device * s_bdev;
    struct hlist_node s_instances;
    struct file_system_type * s_type;
    char s_id[32];
    struct list_head s_list;
    int s_flags;

    //struct rw_semaphore s_umount;//ref count lock 

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

};



struct dentry *mount_bdev(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data,
	int (*fill_super)(struct super_block *, void *, int));

void kill_block_super(struct super_block *sb);


struct super_operations {
    //TODO:
};

// put_super
// drop_super
// iterato_super


#endif   /* SUPER_H */
