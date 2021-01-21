#include "super.h"

#include "base.h"
#include "filesystem.h"
#include "dcache.h"

static LIST_HEAD(super_blocks);
// static DEFINE_SPINLOCK(sb_lock);

// ********  Block device
struct block_device {
    char dev_name[32];
    int mode;
    unsigned bd_block_size;
    struct super_block * bd_super;
    struct mutex bd_fsfreeze_mutex;
    int bd_fsfreeze_count;
};

static inline unsigned int blksize_bits(unsigned int size)
{
    unsigned int bits = 8;
    do {
        bits++;
        size >>= 1;
    } while (size > 256);
    return bits;
}

static inline unsigned int block_size(struct block_device *bdev)
{
        return bdev->bd_block_size;
}

struct block_device * blkdev_get(const char * dev_name, int mode, struct file_system_type * type)
{
    //TODO: create it now
    struct block_device * ret = (struct block_device*)calloc(sizeof(struct block_device), 1);
    if(ret == NULL){
        return ERR_PTR(-ENOMEM);
    }
    strncpy(ret->dev_name, dev_name, sizeof(ret->dev_name));
    ret->mode = mode;
    mutex_init(&ret->bd_fsfreeze_mutex);
    return ret;
}
void blkdev_put(struct block_device * bdev, int mode){
    free(bdev);
}



int set_blocksize(struct block_device *bdev, int size)
{
	/* Size must be a power of two, and between 512 and PAGE_SIZE */
	if (size > PAGE_SIZE || size < 512 || !is_power_of_2(size))
		return -EINVAL;

	/* Size cannot be smaller than the size supported by the device */
	if (size < 512)
		return -EINVAL;

	/* Don't change the size if it is same as current */
	if (bdev->bd_block_size != size) {
		//TODO: sync_blockdev(bdev);
		bdev->bd_block_size = size;
		//TODO: bdev->bd_inode->i_blkbits = blksize_bits(size);
		//TODO: kill_bdev(bdev);
	}
	return 0;
}

int sb_set_blocksize(struct super_block *sb, int size)
{
    if (set_blocksize(sb->s_bdev, size))
        return 0;
    /*  If we get here, we know size is power of two
     *       * and it's value is between 512 and PAGE_SIZE */
    sb->s_blocksize = size;
    sb->s_blocksize_bits = blksize_bits(size);
    return sb->s_blocksize;
}


static void destroy_unused_super(struct super_block * sb){
    if(!sb)
        return ;
    free(sb);
}
static struct super_block * alloc_super(struct file_system_type * type, int flags)
{
    struct super_block * sb = (struct super_block *)calloc(sizeof(struct super_block), 1);
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


    return sb;
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
    if(test){
        hlist_for_each_entry(old, &type->fs_supers, s_instances){
            if(!test(old, data))
                continue;
            destroy_unused_super(sb); 
            return old;
        }
    }
    if(!sb){
        sb = alloc_super(type, flags);
        if(!sb)
            return ERR_PTR(-ENOMEM);
        goto retry;
    }

    err = set(sb, data);
    if(err){
        destroy_unused_super(sb);
        return ERR_PTR(err);
    }

    sb->s_type = type;
    strlcpy(sb->s_id, type->name, sizeof(sb->s_id));
    list_add_tail(&sb->s_list, &super_blocks);
    hlist_add_head(&sb->s_instances, &type->fs_supers);
    get_filesystem(type);
    return sb;
}


static int test_dev(struct super_block * sb, void *bdev){
    return (void *)sb->s_bdev == bdev;
}
static int set_dev(struct super_block * sb, void *bdev){
    sb->s_bdev = bdev;
    //sb->s_dev = sb->s_bdev->bd_dev;
    //s->s_bdi = bdi_get(s->s_bdev->bd_bdi);
    return 0;
}

static void deactivate_locked_super(struct super_block *sb)
{
    //TODO:
    //struct file_system_type *fs = sb->s_type;
    if (atomic_dec_and_test(&sb->s_active)) {
//        cleancache_invalidate_fs(sb);
//        unregister_shrinker(&s->s_shrink);
//        fs->kill_sb(s);
//
//        /*
//         * Since list_lru_destroy() may sleep, we cannot call it from
//         * put_super(), where we hold the sb_lock. Therefore we destroy_unused_super         * the lru lists right now.
//         */
//        list_lru_destroy(&s->s_dentry_lru);
//        list_lru_destroy(&s->s_inode_lru);
//
//        put_filesystem(fs);
//        put_super(s);
//    } else {
//        up_write(&s->s_umount);
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
