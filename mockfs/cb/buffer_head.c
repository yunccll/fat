#include "buffer_head.h"
#include "super.h"
#include "base.h"
#include "block_dev.h"
#include "device.h"

/*
static struct buffer_head * alloc_buffer_head(void){
	struct buffer_head * bh = (struct buffer_head*)calloc(sizeof(struct buffer_head), 1);	
	if(bh){
		INIT_LIST_HEAD(&bh->b_assoc_buffers);
		spin_lock_init(&bh->b_uptodate_lock);
	}
    return bh;
}*/



 





struct buffer_head * sb_bread(struct super_block *sb, sector_t block)
{
    //return __bread_gfp(sb->s_bdev, block, sb->s_blocksize, __GFP_MOVABLE);
    struct block_device * bdev = sb->s_bdev;
    int bytes = 0;
    loff_t offset = 0;
    char buf[512];  

    offset = (block * bdev->bd_block_size);
    bytes = mcfs_dev_read(bdev->bd_real_dev, offset, buf, bdev->bd_block_size);
    if(bytes >= 0){
        pr_debug("read bytes:[%d]\n", bytes);
        //TODO: construct an buffer_head, return it;
        return (void *)1024;
    }
    pr_error("ERROR: read bytes from real device:[%s] faield\n", bdev->dev_name);
    return 0;
}
static inline void put_bh(struct buffer_head *bh)
{
    //smp_mb__before_atomic();
    atomic_dec(&bh->b_count);
}
void __brelse(struct buffer_head * buf){
    if (atomic_read(&buf->b_count)) {
        put_bh(buf);
        return;
    }
    // WARN(1, KERN_ERR "VFS: brelse: Trying to free free buffer\n");
}

void map_bh(struct buffer_head *bh, struct super_block *sb, sector_t block)
{
//TODO:    set_buffer_mapped(bh);
    bh->b_bdev = sb->s_bdev;
    bh->b_blocknr = block;
    bh->b_size = sb->s_blocksize;
}
