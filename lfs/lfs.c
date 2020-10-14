#include <linux/module.h> 
#include <linux/init.h>   
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>


#define LFS_SUPER_MAGIC	0x4d44

struct lfs_super_block {
    int a;
    //dir_iops;
};

static struct lfs_super_block * lfs_super_block_create(void){
    return (struct lfs_super_block*) kzalloc(sizeof(struct lfs_super_block), GFP_KERNEL); 
}
static void lfs_super_block_free(struct lfs_super_block * lfs_sb){
    kfree(lfs_sb);
}


#define FAT_DATE_MIN (0<<9 | 1<<5 | 1)
#define FAT_DATE_MAX (127<<9 | 12<<5 | 31)


static int lfs_fill_super(struct super_block *sb, void *data, int silent){
    sb->s_fs_info = lfs_super_block_create();

    sb->s_flags |= SB_NODIRATIME;
    sb->s_flags |= SB_NOATIME;
    sb->s_magic = LFS_SUPER_MAGIC;
    sb_min_blocksize(sb, 512);
    //TODO: sb->s_op = &fat_sops;
    //sb->s_export_op = &fat_export_ops;
    //sb->s_time_gran = 1;
    //TODO: MSDOS_SB(sb)->dir_ops = &msdos_dir_inode_operations;
    //TODO: sb->s_d_op = &msdos_dentry_operations;
    //sb->s_time_min = ts.tv_sec
    //sb->s_time_max = ts.tv_sec;
    sb->s_maxbytes = 0xffffffff;

     
    //TODO:
    return 0;
}

static struct dentry * lfs_mount(struct file_system_type * fs_type, int flags, const char * dev_name, void * data){
    printk(KERN_WARNING "mounting lfs");
    return mount_bdev(fs_type, flags, dev_name, data, lfs_fill_super);
}

static void lfs_umount(struct super_block * sb){
    kill_block_super(sb);
    lfs_super_block_free((struct lfs_super_block*)sb->s_fs_info);

    printk(KERN_WARNING "umounted lfs");
}


static struct file_system_type lfs_fs_type = {
    .owner      = THIS_MODULE,
    .name       = "lfs",
    .mount      = lfs_mount,
    .kill_sb    = lfs_umount,
    .fs_flags   = FS_REQUIRES_DEV,
};
 
static int __init lfs_init(void)
{
    printk(KERN_WARNING "hello, lfs.\n");
    return register_filesystem(&lfs_fs_type);
}
static void __exit lfs_exit(void)
{
    printk(KERN_WARNING "lfs exit!\n");
    unregister_filesystem(&lfs_fs_type);
}
 


module_init(lfs_init);
module_exit(lfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chenglun");
MODULE_DESCRIPTION("practise code for lfs kernel module");
