#include "block_dev.h"

#include "mock/linux/err.h"
#include "mock/linux/printk.h"
#include "mock/linux/log2.h"
#include "mock/linux/string.h"
#include "mock/linux/slab.h"
#include "mock/asm-generic/page.h"
#include "mock/uapi/linux/fs.h"

#include "super.h"
#include "inode.h"
#include "device.h"

static unsigned int blksize_bits(unsigned int size)
{
    unsigned int bits = 8;
    do {
        bits++;
        size >>= 1;
    } while (size > 256);
    return bits;
}

unsigned int block_size(struct block_device *bdev)
{
        return bdev->bd_block_size;
}





//single block_devcie
struct block_device * blkdev_get(const char * dev_name, int mode, struct file_system_type * type)
{
    struct block_device * ret = (struct block_device*)kzalloc(sizeof(struct block_device), GFP_KERNEL);
    if(ret == NULL){
        return ERR_PTR(-ENOMEM);
    }
    strncpy(ret->dev_name, dev_name, sizeof(ret->dev_name));
    ret->mode = mode;
    mutex_init(&ret->bd_fsfreeze_mutex);
    ret->bd_block_size = BLOCK_SIZE;

    //TODO: create file as device now
    ret->bd_real_dev = mcfs_dev_create_file(ret->dev_name);
    if(!ret->bd_real_dev){
        pr_error("create real device:[%s] failed\n", ret->dev_name);
        goto failed;
    }
    return ret;
    
failed:
    kfree(ret); 
    return NULL;
}
void blkdev_put(struct block_device * bdev, int mode){
    if(bdev){
        mcfs_dev_destroy(bdev->bd_real_dev);
    }
    kfree(bdev);
}



int set_blocksize(struct block_device *bdev, int size)
{
	/* Size must be a power of two, and between BLOCK_SIZE and PAGE_SIZE */
	if (size > PAGE_SIZE || size < BLOCK_SIZE || !is_power_of_2(size))
		return -EINVAL;

	/* Size cannot be smaller than the size supported by the device */
	if (size < BLOCK_SIZE)
		return -EINVAL;

	/* Don't change the size if it is same as current */
	if (bdev->bd_block_size != size) {
		//TODO: sync_blockdev(bdev);
		bdev->bd_block_size = size;
		//TODO: ??  bd_inode == NULL?????   bdev->bd_inode->i_blkbits = blksize_bits(size);
		//TODO: kill_bdev(bdev);
	}
	return 0;
}

int sb_set_blocksize(struct super_block *sb, int size)
{
    if (set_blocksize(sb->s_bdev, size))
        return 0;
    /*  If we get here, we know size is power of two
     * and it's value is between BLOCK_SIZE and PAGE_SIZE */
    sb->s_blocksize = size;
    sb->s_blocksize_bits = blksize_bits(size);
    return sb->s_blocksize;
}

static inline unsigned int bdev_logical_block_size(struct block_device *bdev)
{
    return BLOCK_SIZE; 
    //return queue_logical_block_size(bdev_get_queue(bdev));
}
int sb_min_blocksize(struct super_block *sb, int size)
{
    int minsize = bdev_logical_block_size(sb->s_bdev);
    if (size < minsize)
        size = minsize;
    return sb_set_blocksize(sb, size);
}

void * getRealDevice(struct block_device * bdev){
    if(bdev){
        return bdev->bd_real_dev;
    }
    return NULL;
}
