#include "super.h"
#include "mock/linux/time64.h"
#include "mock/linux/err.h"
#include "mock/linux/errno.h"
#include "mock/linux/printk.h"
#include "mock/linux/string.h"
#include "mock/linux/slab.h"
#include "mock/linux/fs.h"

#include "filesystem.h"
#include "dcache.h"
#include "block_dev.h"

static LIST_HEAD(super_blocks);
static DEFINE_SPINLOCK(sb_lock);


static void destroy_unused_super(struct super_block * sb){
    if(!sb)
        return ;
    kfree(sb);
}
static struct super_block * alloc_super(struct file_system_type * type, int flags)
{
    struct super_block * sb = (struct super_block *)kzalloc(sizeof(struct super_block), GFP_KERNEL);
    static const struct super_operations default_op;

    if(sb == NULL){
        return ERR_PTR(-ENOMEM);
    }

    init_rwsem(&sb->s_umount);
    INIT_HLIST_NODE(&sb->s_instances);
    INIT_LIST_HEAD(&sb->s_list);
    INIT_LIST_HEAD(&sb->s_inodes); //all inodes 

    sb->s_count = 1;
    atomic_set(&sb->s_active, 1);

    sb->s_maxbytes = MAX_NON_LFS;
    sb->s_op = &default_op;
    sb->s_time_gran = 1000000000;
    sb->s_time_min = TIME64_MIN;
    sb->s_time_max = TIME64_MAX;


    spin_lock_init(&sb->s_inode_list_lock);
    INIT_LIST_HEAD(&sb->s_inodes);

    atomic64_set(&sb->s_remove_count, 0);


    return sb;
}

static void __put_super(struct super_block *sb)
{
    if (!--sb->s_count) {
        list_del_init(&sb->s_list);
//        WARN_ON(s->s_dentry_lru.node);
//        WARN_ON(s->s_inode_lru.node);
//        WARN_ON(!list_empty(&s->s_mounts));
//        security_sb_free(s);
//        fscrypt_sb_free(s);
//        put_user_ns(s->s_user_ns);
//        kfree(s->s_subtype);
//        call_rcu(&s->rcu, destroy_super_rcu);
		kfree(sb);
    }
}
static void put_super(struct super_block *sb)
{
    spin_lock(&sb_lock);
    __put_super(sb);
    spin_unlock(&sb_lock);
}

static int grab_super(struct super_block *sb)
{
    sb->s_count++;
    spin_unlock(&sb_lock);
    down_write(&sb->s_umount);
    if ((sb->s_flags & SB_BORN) && atomic_inc_not_zero(&sb->s_active)) {
        put_super(sb);
        return 1;
    }
    up_write(&sb->s_umount);
    put_super(sb);
    return 0;
}


static struct super_block * sget(struct file_system_type * type, 
    int (*test)(struct super_block *, void *), 
    int (*set)(struct super_block *, void *), 
    int flags, 
    void * data)
{
    struct super_block * sb = NULL;
    struct super_block * old;
    int err;
retry:
    spin_lock(&sb_lock);
    if(test){
        hlist_for_each_entry(old, &type->fs_supers, s_instances){
            if(!test(old, data))
                continue;
            if(!grab_super(old)){ // spin_unlock(&sb_lock)
                goto retry;
            }
            destroy_unused_super(sb); 
            return old;
        }
    }
    if(!sb){
		spin_unlock(&sb_lock);
        sb = alloc_super(type, (flags & ~SB_SUBMOUNT));
        if(!sb)
            return ERR_PTR(-ENOMEM);
        goto retry;
    }

    err = set(sb, data);
    if(err){
		spin_unlock(&sb_lock);
        destroy_unused_super(sb);
        return ERR_PTR(err);
    }

    sb->s_type = type;
    strlcpy(sb->s_id, type->name, sizeof(sb->s_id));
    list_add_tail(&sb->s_list, &super_blocks);
    hlist_add_head(&sb->s_instances, &type->fs_supers);
	spin_unlock(&sb_lock);
    get_filesystem(type);
    return sb;
}


static int test_dev(struct super_block * sb, void *bdev){
    return (void *)sb->s_bdev == bdev;
}
static int set_dev(struct super_block * sb, void *bdev){
    sb->s_bdev = bdev;
    sb->s_dev = sb->s_bdev->bd_dev;
    //s->s_bdi = bdi_get(s->s_bdev->bd_bdi);
    return 0;
}

static void deactivate_locked_super(struct super_block *sb)
{
    struct file_system_type *fs = sb->s_type;
    if (atomic_dec_and_test(&sb->s_active)) {
        if(fs->kill_sb)
            fs->kill_sb(sb);

//        /*
//         * Since list_lru_destroy() may sleep, we cannot call it from
//         * put_super(), where we hold the sb_lock. Therefore we destroy_unused_super         * the lru lists right now.
//         */
//        list_lru_destroy(&s->s_dentry_lru);
//        list_lru_destroy(&s->s_inode_lru);

        put_filesystem(fs);
        put_super(sb);
    } else {
        up_write(&sb->s_umount);//TODO: ????
    }
}

struct dentry *mount_bdev(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data,
	int (*fill_super)(struct super_block *, void *, int))
{
    struct block_device * bdev;
    struct super_block * sb;
    int error = 0;
    fmode_t mode = FMODE_READ | FMODE_EXCL;

	pr_info("mount_bdev .....\n");
	// 1. create & open dev_name 
	bdev = blkdev_get(dev_name, 0, fs_type);
    if(IS_ERR(bdev)){
        pr_error("ERROR: blkdev get failed\n");
        return ERR_CAST(bdev);
    }
    mutex_lock(&bdev->bd_fsfreeze_mutex);
    if(bdev->bd_fsfreeze_count > 0){
        mutex_unlock(&bdev->bd_fsfreeze_mutex); 
        error = -EBUSY;
        goto error_bdev;
    }


	// 2. get or create sb with the block_device
    sb = sget(fs_type, test_dev, set_dev, flags, bdev);
    mutex_unlock(&bdev->bd_fsfreeze_mutex);
    if(IS_ERR(sb)){
        pr_error("ERROR: sget failed\n");
        goto error_sb;
    }

    if(sb->s_root){ // find old sb, and free the new get blkdev 
        // exception  ....
        
        up_write(&sb->s_umount);
        blkdev_put(bdev, mode);
        down_write(&sb->s_umount);
    }
    else{ // alloc new sb , and init the new sb 
        sb->s_mode = mode;
        snprintf(sb->s_id, sizeof(sb->s_id), "%s", (const char *)(bdev->dev_name)); //TBD:  snprintf("%pg") ???
        sb_set_blocksize(sb, block_size(bdev));
        error = fill_super(sb, data, flags & SB_SILENT ? 1 : 0);
        if (error) {
            deactivate_locked_super(sb);
            goto error_bdev;
        }
        sb->s_flags |= SB_ACTIVE;
        bdev->bd_super = sb;
    }
	return dget(sb->s_root);

error_sb:
    error = PTR_ERR(sb);
error_bdev:
    blkdev_put(bdev, mode);
	return ERR_PTR(error);
}

void kill_block_super(struct super_block *sb)
{
	struct block_device *bdev = sb->s_bdev;
	fmode_t mode = sb->s_mode;
	bdev->bd_super = NULL;

	pr_info("kill_block_super\n");
	//TODO:
//	generic_shutdown_super(sb);
//	sync_blockdev(bdev);
//	WARN_ON_ONCE(!(mode & FMODE_EXCL));
	blkdev_put(bdev, mode | FMODE_EXCL);
}
