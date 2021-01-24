#include "block_dev.h"
#include "super.h"
#include "inode.h"
#include "base.h"
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
    struct block_device * ret = (struct block_device*)calloc(sizeof(struct block_device), 1);
    if(ret == NULL){
        return ERR_PTR(-ENOMEM);
    }
    strncpy(ret->dev_name, dev_name, sizeof(ret->dev_name));
    ret->mode = mode;
    mutex_init(&ret->bd_fsfreeze_mutex);
    ret->bd_block_size = 512;

    //TODO: create file as device now
    ret->bd_real_dev = mcfs_dev_create_file(ret->dev_name);
    if(!ret->bd_real_dev){
        pr_error("create real device:[%s] failed\n", ret->dev_name);
        goto failed;
    }
    return ret;
    
failed:
    free(ret); 
    return NULL;
}
void blkdev_put(struct block_device * bdev, int mode){
    if(bdev){
        mcfs_dev_destroy(bdev->bd_real_dev);
    }
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
     * and it's value is between 512 and PAGE_SIZE */
    sb->s_blocksize = size;
    sb->s_blocksize_bits = blksize_bits(size);
    return sb->s_blocksize;
}

static inline unsigned int bdev_logical_block_size(struct block_device *bdev)
{
    return 512; 
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
