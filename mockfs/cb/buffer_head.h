
#ifndef  BUFFER_HEAD_H
#define  BUFFER_HEAD_H

#include <stddef.h>
#include "list.h"

#include "types.h"
#include "lock.h"

struct super_block;
struct block_device;
struct buffer_head {

    sector_t b_blocknr;     /* start block number, default -1 */
    struct block_device * b_bdev;

    struct buffer_head *b_this_page;/* circular list of page's buffers */

    char *b_data;           /* pointer to data within the page */
    size_t b_size;		/* size of mapping */

    struct list_head b_assoc_buffers; /* associated with another mapping */
    spinlock_t b_uptodate_lock; /* Used by the first bh in a page, to
                     * serialise IO completion of other
                     * buffers in the page */

    atomic_t b_count;       /* users using this buffer_head */


//    unsigned long b_state;      /* buffer state bitmap (see above) */
//    struct page *b_page;        /* the page this bh is mapped to */
//
//
//    bh_end_io_t *b_end_io;      /* I/O completion */
//    void *b_private;        /* reserved for b_end_io */
//    struct address_space *b_assoc_map;  /* mapping this buffer is
//                           associated with */
};

//TODO: create buffer_head ?????
struct buffer_head * sb_bread(struct super_block *sb, sector_t block);
void __brelse(struct buffer_head *);
static inline void brelse(struct buffer_head *bh)
{
    if (bh)
        __brelse(bh);
}

void map_bh(struct buffer_head *bh, struct super_block *sb, sector_t block);


#endif   /* BUFFER_HEAD_H */
