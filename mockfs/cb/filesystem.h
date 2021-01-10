
#ifndef  FILESYSTEM_H
#define  FILESYSTEM_H

struct dentry;
struct super_block;

struct file_system_type {
    const char *name;
    int fs_flags;
#define FS_REQUIRES_DEV     1
#define FS_BINARY_MOUNTDATA 2
#define FS_HAS_SUBTYPE      4
#define FS_USERNS_MOUNT     8   /* Can be mounted by userns root */
#define FS_DISALLOW_NOTIFY_PERM 16  /* Disable fanotify permission events */
#define FS_RENAME_DOES_D_MOVE   32768   /* FS will handle d_move() during rename() internally. */
    //int (*init_fs_context)(struct fs_context *);
    //const struct fs_parameter_description *parameters;
    struct dentry *(*mount) (struct file_system_type *, int,
               const char *, void *);
    void (*kill_sb) (struct super_block *);
    //struct module *owner;
    struct file_system_type * next;
    //struct hlist_head fs_supers;
};


int register_filesystem(struct file_system_type * fs);
int unregister_filesystem(struct file_system_type * fs);

void print_filesystem(void);

#endif   /* FILESYSTEM_H */
