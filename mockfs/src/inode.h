#ifndef  INODE_H
#define  INODE_H

#include "mock/linux/types.h"
#include "mock/linux/list.h"
#include "mock/linux/lock.h"
#include "mock/linux/time64.h"


struct super_block;



/*
 * Inode state bits.  Protected by inode->i_lock
 *
 * Three bits determine the dirty state of the inode, I_DIRTY_SYNC,
 * I_DIRTY_DATASYNC and I_DIRTY_PAGES.
 *
 * Four bits define the lifetime of an inode.  Initially, inodes are I_NEW,
 * until that flag is cleared.  I_WILL_FREE, I_FREEING and I_CLEAR are set at
 * various stages of removing an inode.
 *
 * Two bits are used for locking and completion notification, I_NEW and I_SYNC.
 *
 * I_DIRTY_SYNC		Inode is dirty, but doesn't have to be written on
 *			fdatasync().  i_atime is the usual cause.
 * I_DIRTY_DATASYNC	Data-related inode changes pending. We keep track of
 *			these changes separately from I_DIRTY_SYNC so that we
 *			don't have to write inode on fdatasync() when only
 *			mtime has changed in it.
 * I_DIRTY_PAGES	Inode has dirty pages.  Inode itself may be clean.
 * I_NEW		Serves as both a mutex and completion notification.
 *			New inodes set I_NEW.  If two processes both create
 *			the same inode, one of them will release its inode and
 *			wait for I_NEW to be released before returning.
 *			Inodes in I_WILL_FREE, I_FREEING or I_CLEAR state can
 *			also cause waiting on I_NEW, without I_NEW actually
 *			being set.  find_inode() uses this to prevent returning
 *			nearly-dead inodes.
 * I_WILL_FREE		Must be set when calling write_inode_now() if i_count
 *			is zero.  I_FREEING must be set when I_WILL_FREE is
 *			cleared.
 * I_FREEING		Set when inode is about to be freed but still has dirty
 *			pages or buffers attached or the inode itself is still
 *			dirty.
 * I_CLEAR		Added by clear_inode().  In this state the inode is
 *			clean and can be destroyed.  Inode keeps I_FREEING.
 *
 *			Inodes that are I_WILL_FREE, I_FREEING or I_CLEAR are
 *			prohibited for many purposes.  iget() must wait for
 *			the inode to be completely released, then create it
 *			anew.  Other functions will just ignore such inodes,
 *			if appropriate.  I_NEW is used for waiting.
 *
 * I_SYNC		Writeback of inode is running. The bit is set during
 *			data writeback, and cleared with a wakeup on the bit
 *			address once it is done. The bit is also used to pin
 *			the inode in memory for flusher thread.
 *
 * I_REFERENCED		Marks the inode as recently references on the LRU list.
 *
 * I_DIO_WAKEUP		Never set.  Only used as a key for wait_on_bit().
 *
 * I_WB_SWITCH		Cgroup bdi_writeback switching in progress.  Used to
 *			synchronize competing switching instances and to tell
 *			wb stat updates to grab the i_pages lock.  See
 *			inode_switch_wbs_work_fn() for details.
 *
 * I_OVL_INUSE		Used by overlayfs to get exclusive ownership on upper
 *			and work dirs among overlayfs mounts.
 *
 * I_CREATING		New object's inode in the middle of setting up.
 *
 * I_SYNC_QUEUED	Inode is queued in b_io or b_more_io writeback lists.
 *			Used to detect that mark_inode_dirty() should not move
 * 			inode between dirty lists.
 *
 * Q: What is the difference between I_WILL_FREE and I_FREEING?
 */
#define I_DIRTY_SYNC		(1 << 0)
#define I_DIRTY_DATASYNC	(1 << 1)
#define I_DIRTY_PAGES		(1 << 2)
#define __I_NEW			3
#define I_NEW			(1 << __I_NEW)
#define I_WILL_FREE		(1 << 4)
#define I_FREEING		(1 << 5)
#define I_CLEAR			(1 << 6)
#define __I_SYNC		7
#define I_SYNC			(1 << __I_SYNC)
#define I_REFERENCED		(1 << 8)
#define __I_DIO_WAKEUP		9
#define I_DIO_WAKEUP		(1 << __I_DIO_WAKEUP)
#define I_LINKABLE		(1 << 10)
#define I_DIRTY_TIME		(1 << 11)
#define I_DIRTY_TIME_EXPIRED	(1 << 12)
#define I_WB_SWITCH		(1 << 13)
#define I_OVL_INUSE		(1 << 14)
#define I_CREATING		(1 << 15)
#define I_SYNC_QUEUED		(1 << 17)

#define I_DIRTY_INODE (I_DIRTY_SYNC | I_DIRTY_DATASYNC)
#define I_DIRTY (I_DIRTY_INODE | I_DIRTY_PAGES)
#define I_DIRTY_ALL (I_DIRTY | I_DIRTY_TIME)

struct inode {
    struct super_block * i_sb;
    struct list_head i_sb_list;


    struct rw_semaphore i_rwsem;

    unsigned int i_flags;
    
    unsigned long i_ino;
    loff_t          i_size;
    struct timespec64   i_atime;
    struct timespec64   i_mtime;
    struct timespec64   i_ctime;

    spinlock_t i_lock;
    unsigned long i_state;
    blkcnt_t        i_blocks;
    unsigned short  i_bytes;
   	u8      i_blkbits;

    umode_t i_mode;
    atomic64_t      i_version; /* NOTE: linux/iversion.h  typical atomic use case for lock-free tech */

	/*
	 * Filesystems may only read i_nlink directly.  They shall use the
	 * following functions for modification:
	 *
	 *    (set|clear|inc|drop)_nlink
	 *    inode_(inc|dec)_link_count
	 */
	union {
		const unsigned int i_nlink;
		unsigned int __i_nlink;
	};

//    const struct inode_operations   *i_op;
//    struct super_block  *i_sb;
//    struct address_space    *i_mapping;
};

struct inode * inode_create();
struct inode * new_inode(struct super_block * sb);
int inode_init_always(struct super_block * sb, struct inode * inode);


void iput(struct inode * inode);


/**
 * drop_nlink - directly drop an inode's link count
 * @inode: inode
 *
 * This is a low-level filesystem helper to replace any
 * direct filesystem manipulation of i_nlink.  In cases
 * where we are attempting to track writes to the
 * filesystem, a decrement to zero means an imminent
 * write when the file is truncated and actually unlinked
 * on the filesystem.
 */
void drop_nlink(struct inode *inode);
/**
 * clear_nlink - directly zero an inode's link count
 * @inode: inode
 *
 * This is a low-level filesystem helper to replace any
 * direct filesystem manipulation of i_nlink.  See
 * drop_nlink() for why we care about i_nlink hitting zero.
 */
void clear_nlink(struct inode *inode);
/**
 * set_nlink - directly set an inode's link count
 * @inode: inode
 * @nlink: new nlink (should be non-zero)
 *
 * This is a low-level filesystem helper to replace any
 * direct filesystem manipulation of i_nlink.
 */
void set_nlink(struct inode *inode, unsigned int nlink);
/**
 * inc_nlink - directly increment an inode's link count
 * @inode: inode
 *
 * This is a low-level filesystem helper to replace any
 * direct filesystem manipulation of i_nlink.  Currently,
 * it is only here for parity with dec_nlink().
 */
void inc_nlink(struct inode *inode);


#endif   /* INODE_H */
