#include <linux/module.h> 
#include <linux/init.h>   
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>

#include <linux/cred.h>

#define LFS_SUPER_MAGIC	0x4d44

struct lfs_sbp {
    int a;
    //dir_iops;
};


/* 
static struct inode *fat_alloc_inode(struct super_block *sb)
{
    struct msdos_inode_info *ei;
    ei = kmem_cache_alloc(fat_inode_cachep, GFP_NOFS);
    if (!ei)
        return NULL;

    init_rwsem(&ei->truncate_lock);
    // Zeroing to allow iput() even if partial initialized inode.
    ei->mmu_private = 0;
    ei->i_start = 0;
    ei->i_logstart = 0;
    ei->i_attrs = 0;
    ei->i_pos = 0;

    return &ei->vfs_inode;
}

static void fat_free_inode(struct inode *inode)
{
    //kmem_cache_free(fat_inode_cachep, MSDOS_I(inode));
}*/



static const struct super_operations lfs_super_ops; 
//{
    //.alloc_inode    = fat_alloc_inode,
    //.free_inode     = fat_free_inode,
    //.write_inode    = fat_write_inode,
    //.evict_inode    = fat_evict_inode,
    //.put_super      = fat_put_super,
    //.statfs         = fat_statfs,
    //.remount_fs     = fat_remount,

    //.show_options   = fat_show_options,
//};

static const struct inode_operations lfs_dir_inode_ops;



static struct lfs_sbp * lfs_sbp_create(void){
    return (struct lfs_sbp*) kzalloc(sizeof(struct lfs_sbp), GFP_KERNEL); 
}
static void lfs_sbp_free(struct lfs_sbp * lfs_sb){
    kfree(lfs_sb);
}

static void lfs_super_block_init_some(struct super_block * sb){
    sb->s_flags |= SB_NODIRATIME;
    sb->s_flags |= SB_NOATIME;
    sb->s_magic = LFS_SUPER_MAGIC;
    sb_min_blocksize(sb, 512);
    sb->s_op = &lfs_super_ops; //TODO:  super_block_operations;
    //sb->s_export_op = &fat_export_ops; //TODO: for nfs
    //sb->s_time_gran = 1;
    //TODO: sb->s_d_op = &msdos_dentry_operations;  //TODO: for dentry_operations
    //sb->s_time_min = ts.tv_sec
    //sb->s_time_max = ts.tv_sec;
    sb->s_maxbytes = 0xffffffff;
}


#define FAT_DATE_MIN (0<<9 | 1<<5 | 1)
#define FAT_DATE_MAX (127<<9 | 12<<5 | 31)
#define MSDOS_ROOT_INO  1 

#define CLUSTER_SIZE 512

static struct inode * init_root_inode(struct inode * root_inode){
    if(root_inode){
        root_inode->i_mode = S_IRWXUGO|S_IFDIR;
        root_inode->i_ino = MSDOS_ROOT_INO;
        root_inode->i_uid = current_uid();
        root_inode->i_gid = current_gid();
        
        root_inode->i_op = &lfs_dir_inode_ops; //TODO: msdos_dir_inode_operations
        root_inode->i_fop = NULL; // TODO: &fat_dir_operations;

        root_inode->i_size = 14 * 512;
        root_inode->i_blocks = ((root_inode->i_size + (CLUSTER_SIZE - 1)) & ~((loff_t)CLUSTER_SIZE - 1)) >> 9;
    }
    return root_inode;
}
static struct inode * make_root_inode(struct super_block * sb){
    struct inode * root_inode = new_inode(sb);
    if(!root_inode) return root_inode;
    return init_root_inode(root_inode);
}
static void delete_inode(struct inode * root_inode){
    //NOOP
}
static void destroy_root_inode(struct inode * root_inode, struct super_block * sb){
    //  d_free_root(root_inode)//free the inode for d_make_root  
    if(root_inode)
        delete_inode(root_inode);
}

static int lfs_fill_super(struct super_block *sb, void *data, int silent){
    struct inode * root_inode;

    int err = -EINVAL;
    sb->s_fs_info = lfs_sbp_create();
    if(!sb->s_fs_info){
        printk(KERN_ERR "create private super block struct failed");
        return -ENOMEM;
    }

    lfs_super_block_init_some(sb);
   
    root_inode = make_root_inode(sb);
    if(root_inode == NULL){
        printk(KERN_ERR "make root inode failed");
        err = -ENOMEM;
        goto free_sbp;
    }
    sb->s_root = d_make_root(root_inode);
    if(!sb->s_root){
        printk(KERN_ERR "make root dentry from inode failed");
        err = -EINVAL;
        goto free_root_inode;
    }
    return 0;

free_root_inode:
    delete_inode(root_inode);
free_sbp:
    lfs_sbp_free(sb->s_fs_info);
    return  err;
}

static void lfs_free_super(struct super_block * sb){ 
    struct dentry * root_dentry = sb->s_root;

    destroy_root_inode(root_dentry->d_inode, sb);
    sb->s_root = NULL;

    lfs_sbp_free((struct lfs_sbp*)sb->s_fs_info); //for lfs_super_block_create
    sb->s_fs_info = NULL;
}

static struct dentry * lfs_mount(struct file_system_type * fs_type, int flags, const char * dev_name, void * data){
    printk(KERN_WARNING "mounting lfs");
    return mount_bdev(fs_type, flags, dev_name, data, lfs_fill_super);
}

static void lfs_umount(struct super_block * sb){
    kill_block_super(sb); //for mount_bdev

    lfs_free_super(sb);// for lfs_fill_super


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
