#include "super.h"

#include "base.h"


struct block_device * blkdev_get()
{
    return NULL;
}
struct super_block * sget(struct file_system_type * type, 
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



struct dentry *mount_bdev(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data,
	int (*fill_super)(struct super_block *, void *, int))
{
	pr_info("mount_bdev, TODO:\n");
	//TODO:
	// 1. create & open dev_name 
	// struct block_device * bdev = blkdev_get();
	//
	// 2. get or create sb with the block_device
    //  sget(fs_type, test_dev, set_dev, flags|SB_NOSEC, bdev);
	//
	// 3. if(s->s_root){ // if get the sb -> free the new get blkdev
	//		blkdev_put(bdev, mode); 
	// }
	// else{
	//		fill_super(sb, data, 0:1);	
	//		s->s_flags |= SB_ACTIVE;
	//		bdev....	
	// }
	// return dget(s->s_root);

//error_bdev:
		//blkdev_put(bdev, mode);
	return (struct dentry*)0;
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
