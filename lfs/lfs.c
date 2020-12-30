#include <linux/module.h> 
#include <linux/init.h>   
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>

#include <linux/cred.h>
#include "lfs.h"

#define LFS_SUPER_MAGIC	0x4d44

#define MSDOS_ROOT_INO  1 
#define FAT_DATE_MIN (0<<9 | 1<<5 | 1)
#define FAT_DATE_MAX (127<<9 | 12<<5 | 31)

#define CLUSTER_SIZE 512

// super block for lfs
struct lfs_sbp {
    int a;
};


// inode for lfs
struct lfs_inode_info {
	//TODO:
	struct inode vfs_inode;
};
static struct lfs_inode_info * lfs_i(struct inode * inode)
{
	return container_of(inode, struct lfs_inode_info, vfs_inode);
}
// inode cache
static struct kmem_cache * lfs_inode_cachep;
static void init_once(void * foo)
{
    struct lfs_inode_info *ei = (struct lfs_inode_info *)foo;
    inode_init_once(&ei->vfs_inode);
}
static int __init lfs_init_inodecache(void)
{
    lfs_inode_cachep = kmem_cache_create("lfs_inode_cache",
                         sizeof(struct lfs_inode_info),
                         0, (SLAB_RECLAIM_ACCOUNT|SLAB_MEM_SPREAD|SLAB_ACCOUNT),
                         init_once);
    if (lfs_inode_cachep == NULL)
        return -ENOMEM;
    return 0;
}

static void __exit lfs_destroy_inodecache(void)
{
    kmem_cache_destroy(lfs_inode_cachep);
}

// inode operations
static struct inode * lfs_alloc_inode(struct super_block *sb)
{
    struct lfs_inode_info *ei;
    ei = kmem_cache_alloc(lfs_inode_cachep, GFP_KERNEL);
    if (!ei)
        return NULL;

	/* TODO:
    init_rwsem(&ei->truncate_lock);
    // Zeroing to allow iput() even if partial initialized inode.
    ei->mmu_private = 0;
    ei->i_start = 0;
    ei->i_logstart = 0;
    ei->i_attrs = 0;
    ei->i_pos = 0;
	*/
    return &ei->vfs_inode;
}

static void lfs_free_inode(struct inode *inode)
{
    kmem_cache_free(lfs_inode_cachep, lfs_i(inode));
}




static const struct super_operations lfs_super_ops = 
{
    .alloc_inode    = lfs_alloc_inode,
    .free_inode     = lfs_free_inode,
    //.write_inode    = fat_write_inode,
    //.evict_inode    = fat_evict_inode,
    //.put_super      = fat_put_super,
    //.statfs         = fat_statfs,
    //.remount_fs     = fat_remount,

    //.show_options   = fat_show_options,
};



static int lfs_create(struct inode *dir, struct dentry *dentry, umode_t mode, bool excl)
{
	struct super_block * sb = dir->i_sb;
	struct inode * inode;

	pr_info("create dentry name:%s, len:%d", dentry->d_name.name, dentry->d_name.len);

	inode = new_inode(sb);
	if(inode){
		pr_err("alloc inode failed");
		return -ENOMEM;
	}
	inode_init_owner(inode, dir, mode);
	inode->i_ino = iunique(sb, MSDOS_ROOT_INO);
	inode->i_mtime = inode->i_atime = inode->i_ctime = current_time(inode);
	inode->i_blocks = 0;

	if (S_ISREG(inode->i_mode)) {
		//inode->i_op = &minix_file_inode_operations;
		//inode->i_fop = &minix_file_operations;
		//inode->i_mapping->a_ops = &minix_aops;
	}
	else if (S_ISDIR(inode->i_mode)) {
		inode->i_op = &lfs_dir_inode_ops;
		inode->i_fop = &lfs_dir_file_operations;
		//inode->i_mapping->a_ops = &minix_aops;
	} 
	// else 		init_special_inode(inode, inode->i_mode, rdev);

	return 0;
}
static struct dentry * lfs_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags)
{
	struct inode * inode;
	struct super_block *sb = dir->i_sb;

	pr_info("lookup dentry name:%s, len:%d", dentry->d_name.name, dentry->d_name.len);

	// find inode or create one
	//inode = iget_locked(sb, ino);
	inode = new_inode(sb);
	if(!inode){
		pr_info("inode is NULL!");
		return ERR_PTR(-ENOMEM);
	}
	if((inode->i_state & I_NEW)){ // if New inode
		inode->i_mode = S_IRWXUGO|S_IFREG;
		inode->i_ino = iunique(sb, MSDOS_ROOT_INO);
        inode->i_uid = current_uid();
        inode->i_gid = current_gid();
		set_nlink(inode, 1);
		inode->i_size = 0;
		inode->i_mtime.tv_sec = 0;
		inode->i_mtime.tv_nsec = 0;
		inode->i_atime.tv_nsec = 0;
		inode->i_ctime.tv_nsec = 0;
		inode->i_blocks = 0;

		if (S_ISREG(inode->i_mode)) {
			pr_info("inode is regular file");
			//inode->i_op = &lfs_file_inode_operations;
			//inode->i_fop = &lfs_file_operations;
			//inode->i_mapping->a_ops = &lfs_aops;
		}
	}
	pr_info("lookup ok");
//	unlock_new_inode(inode);
	return d_splice_alias(inode, dentry);
}


const struct inode_operations lfs_dir_inode_ops = {
	.create     = lfs_create,
    .lookup     = lfs_lookup,
    //.unlink     = msdos_unlink,
    //.mkdir      = msdos_mkdir,
    //.rmdir      = msdos_rmdir,
    //.rename     = msdos_rename,
    //.setattr    = fat_setattr,
    //.getattr    = fat_getattr,
    //.update_time    = fat_update_time,
};



static struct lfs_sbp * lfs_sbp_create(void){
    return (struct lfs_sbp*) kzalloc(sizeof(struct lfs_sbp), GFP_KERNEL); 
}
static void lfs_sbp_free(struct lfs_sbp * lfs_sb){
    kfree(lfs_sb);
}



/********    Dentry Operations for  *******************/
static int lfs_dentry_hash(const struct dentry *dentry, struct qstr *qstr)
{
	pr_info("qstr:%s:%d", qstr->name, qstr->len);
	qstr->hash = full_name_hash(dentry, qstr->name, qstr->len);
    return 0;
}
static int lfs_dentry_cmp(const struct dentry *dentry, unsigned int len, const char *str, const struct qstr *name)
{
	pr_info("len:%d,str:%s", len, str);
	pr_info("qstr:%s:%d", name->name, name->len);
    return memcmp(name->name, str, len);
}

static const struct dentry_operations lfs_dentry_operations = {
    .d_hash     = lfs_dentry_hash,
    .d_compare  = lfs_dentry_cmp,
};


static int lfs_readdir(struct file *file, struct dir_context *ctx)
{
	if(ctx->pos == 0){
		dir_emit(ctx, "a.txt", 5, 100, DT_UNKNOWN);
		ctx->pos += 1;
	}
	return 0;
}


const struct file_operations lfs_dir_file_operations = {
    .llseek     = generic_file_llseek,
    .read       = generic_read_dir,
    .iterate_shared = lfs_readdir,
    //.unlocked_ioctl = lfs_dir_ioctl,
    //.fsync      = lfs_file_fsync,
};



static void lfs_super_block_init_some(struct super_block * sb){
    sb->s_flags |= SB_NODIRATIME;
    sb->s_flags |= SB_NOATIME;
    sb->s_magic = LFS_SUPER_MAGIC;
    sb_min_blocksize(sb, 512);
    sb->s_op = &lfs_super_ops;
    sb->s_d_op = &lfs_dentry_operations;
    sb->s_time_gran = 1;
    //sb->s_export_op = &fat_export_ops; //TODO: for nfs
    //sb->s_time_min = ts.tv_sec
    //sb->s_time_max = ts.tv_sec;
    sb->s_maxbytes = 0xffffffff;
}

static struct inode * init_root_inode(struct inode * root_inode){
    if(root_inode){
        root_inode->i_mode = S_IRWXUGO|S_IFDIR;
        root_inode->i_ino = MSDOS_ROOT_INO;
        root_inode->i_uid = current_uid();
        root_inode->i_gid = current_gid();
        
        root_inode->i_op = &lfs_dir_inode_ops;
        root_inode->i_fop = &lfs_dir_file_operations;

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
    free_inode_nonrcu(inode);
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
	lfs_init_inodecache();

    return register_filesystem(&lfs_fs_type);
}
static void __exit lfs_exit(void)
{
    printk(KERN_WARNING "lfs exit!\n");
    unregister_filesystem(&lfs_fs_type);
	lfs_destroy_inodecache();
}
 


module_init(lfs_init);
module_exit(lfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("chenglun");
MODULE_DESCRIPTION("practise code for lfs kernel module");
