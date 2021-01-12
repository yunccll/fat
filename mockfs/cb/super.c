#include "super.h"

#include "base.h"


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


static struct super_block * sget(struct file_system_type * type, 
    int (*test)(struct super_block *, void *), 
    int (*set)(struct super_block *, void *), 
    int flags, 
    void * data)
{
    //lock 
    // search s from fs_type->fs_supers;
    // find or not
    //  find -> grab_super && return super 
    //  not find -> {  
    //      alloc & init the new sb
    //      add sb to  sb_list and type
    //  }
    return 0;
}

static int test_dev(struct super_block * sb, void *bdev){
    //TODO:
    return 0;
}
static int set_dev(struct super_block * sb, void *bdev){
    //TODO:
    return 0;
}


struct dentry *mount_bdev(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data,
	int (*fill_super)(struct super_block *, void *, int))
{
    struct block_device * bdev;
    struct super_block * sb;
    int error = 0;
	pr_info("mount_bdev .....\n");
	// 1. create & open dev_name 
	bdev = blkdev_get(dev_name, 0, fs_type);
    if(IS_ERR(bdev)){
        pr_error("ERROR: blkdev get failed\n");
        return ERR_CAST(bdev);
    }
	//TODO:
	// 2. get or create sb with the block_device
    sb = sget(fs_type, test_dev, set_dev, flags, bdev);
    if(IS_ERR(sb)){
        pr_error("ERROR: sget failed\n");
        goto error_sb;
    }
	// 3. if(s->s_root){ // if get the sb -> free the new get blkdev
	//		blkdev_put(bdev, mode); 
	// }
	// else{
	//		fill_super(sb, data, 0:1);	
	//		s->s_flags |= SB_ACTIVE;
	//		bdev....	
	// }
	// return dget(s->s_root);

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
