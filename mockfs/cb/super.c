#include "super.h"

#include "base.h"
#include "filesystem.h"
#include "dcache.h"

static LIST_HEAD(super_blocks);
// static DEFINE_SPINLOCK(sb_lock);

// ********  Block device
struct block_device {
    char dev_name[512];
    int mode;
};

struct block_device * blkdev_get(const char * dev_name, int mode, struct file_system_type * type)
{
    //TODO: create it now
    struct block_device * ret = (struct block_device*)calloc(sizeof(struct block_device), 1);
    if(ret == NULL){
        return ERR_PTR(-ENOMEM);
    }
    strncpy(ret->dev_name, dev_name, sizeof(ret->dev_name));
    ret->mode = mode;
    return ret;
}
void blkdev_put(struct block_device * bdev, int mode){
    free(bdev);
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


    //init_rwsem(&s->s_umount);
    INIT_HLIST_NODE(&sb->s_instances);
    INIT_LIST_HEAD(&sb->s_list);

    sb->s_count = 1;
    atomic_set(&sb->s_active, 1);

    sb->s_maxbytes = MAX_NON_LFS;
    sb->s_op = &default_op;
    sb->s_time_gran = 1000000000;
    sb->s_time_min = TIME64_MIN;
    sb->s_time_max = TIME64_MAX;

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
    //get_filesystem(type); //TODO: sb ptr to type , add refcount of type struct
    return sb;
}


static int test_dev(struct super_block * sb, void *bdev){
    //TODO: return (void *)s->s_bdev == data;
    return 0;
}
static int set_dev(struct super_block * sb, void *bdev){
    //TODO:
    //s->s_bdev = data;
    //s->s_dev = s->s_bdev->bd_dev;
    //s->s_bdi = bdi_get(s->s_bdev->bd_bdi);
    return 0;
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
	// 2. get or create sb with the block_device
    sb = sget(fs_type, test_dev, set_dev, flags, bdev);
    if(IS_ERR(sb)){
        pr_error("ERROR: sget failed\n");
        goto error_sb;
    }
    if(sb->s_root){
        //up_write(&s->s_umount);
        blkdev_put(bdev, mode);
        //down_write(&s->s_umount);
    }
    else{
        sb->s_mode = mode;
	//TODO:
//        snprintf(s->s_id, sizeof(s->s_id), "%pg", bdev);
//        sb_set_blocksize(s, block_size(bdev));
//        error = fill_super(s, data, flags & SB_SILENT ? 1 : 0);
//        if (error) {
//            deactivate_locked_super(s);
//            goto error;
//        }
//
        sb->s_flags |= SB_ACTIVE;
//        bdev->bd_super = sb;
    }
	return dget(sb->s_root);

error_sb:
    error = PTR_ERR(sb);
    blkdev_put(bdev, 0);
	return ERR_PTR(error);
}

void kill_block_super(struct super_block *sb)
{
	pr_info("kill_block_super\n");
	//TODO:
//	struct block_device *bdev = sb->s_bdev;
//	fmode_t mode = sb->s_mode;
//
//	bdev->bd_super = NULL;
//	generic_shutdown_super(sb);
//	sync_blockdev(bdev);
//	WARN_ON_ONCE(!(mode & FMODE_EXCL));
//	blkdev_put(bdev, mode | FMODE_EXCL);
}
