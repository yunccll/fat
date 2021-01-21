#include "buffer_head.h"
#include "super.h"

struct buffer_head * sb_bread(struct super_block *sb, sector_t block)
{
    //TODO:
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
