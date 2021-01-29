#include "buffer_head.h"
#include "super.h"
#include "base.h"
#include "block_dev.h"
#include "device.h"


static struct buffer_head * alloc_buffer_head(void){
	struct buffer_head * bh = (struct buffer_head*)calloc(sizeof(struct buffer_head), 1);	
	if(bh){
		INIT_LIST_HEAD(&bh->b_assoc_buffers);
		spin_lock_init(&bh->b_uptodate_lock);
        //TODO: bh->b_count = 0;
        bh->b_blocknr = -1;
        bh->b_bdev = 0;
        bh->b_this_page = 0;
        bh->b_data = 0;
        bh->b_size = 0;
	}
    return bh;
}

static void free_buffer_head(struct buffer_head * bh){
    free(bh); 
}

/*
struct page {
    char addr[PAGE_SIZE];
};
static char * address_page(struct * page) 
{
    return page->addr;
}

// page -> multi-buffers
static buffer_head * alloc_page_buffer(struct * page, unsigned long size, bool retry)
{
    struct buffer_head * head = NULL, bh = NULL;
    long offset = PAGE_SIZE;

    while((offset -= size) > 0){
        bh = alloc_buffer_head();
        if(!bh)
            goto no_grow;

        bh->b_this_page = page;
        bh->b_blocknr = -1;
        head = bh;
        bh->b_size = size;

        bh->b_page = page;
        bh->b_data = address_page(page) + offset;
    }
    return head;

no_grow:
    if(head) {
        do {
            bh = head; 
            head = head->b_this_page;
            free_buffer_head(bh);
        } while(head)
        return head;
    }
}*/

static void free_block_buffer(struct buffer_head * bh){
    if(bh){
        if(bh->b_data){
            free(bh->b_data);
        }
        free_buffer_head(bh);
    }
}
static struct buffer_head * alloc_block_buffer(unsigned long size)
{
    struct buffer_head * head = alloc_buffer_head();
    if(head){
        head->b_data = (char*)calloc(sizeof(char), size);
        head->b_size = size;
    }
    return head;
}

struct buffer_head * sb_bread(struct super_block *sb, sector_t block)
{
    //return __bread_gfp(sb->s_bdev, block, sb->s_blocksize, __GFP_MOVABLE);
    struct block_device * bdev = sb->s_bdev;
    int bytes = 0;
    loff_t offset = 0;
    struct buffer_head * bh = alloc_block_buffer(bdev->bd_block_size);
    if(!bh){
        pr_error("ERROR: no enough memroy");
        return NULL;
    }
    bh->b_blocknr = block;

    offset = (block * bdev->bd_block_size);
    bytes = mcfs_dev_read(bdev->bd_real_dev, offset, bh->b_data, bdev->bd_block_size);
    if(bytes >= 0){
        pr_debug("read bytes:[%d]\n", bytes);
        return bh;
    }
    free_block_buffer(bh);
    pr_error("ERROR: read bytes from real device:[%s] faield\n", bdev->dev_name);
    return NULL;
}
static inline void put_bh(struct buffer_head *bh)
{
    //smp_mb__before_atomic();
    atomic_dec(&bh->b_count);
    if(atomic_dec_and_test(&bh->b_count)){
        free_block_buffer(bh);
    }
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
