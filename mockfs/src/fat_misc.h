
#ifndef  FAT_MISC_H
#define  FAT_MISC_H

#include "mock/linux/types.h"

struct fat_sb;
struct timespec64;

void fat_time_fat2unix(struct fat_sb *fsb, struct timespec64 *ts,
		__le16 __time, __le16 __date, u8 time_cs);


/* fat_attr ---> inode->imode;
 *                          is_dir?     writable
 * ATTR_NONE    (0) -->         X       
 * ATTR_RO      (1) -->         X       &= ~I_IWUGO
 * ATTR_HIDDEN  (2) -->         X
 * ATTR_SYS     (4) -->         X
 * ATTR_VOLUME  (8) -->         X
 * ATTR_DIR     (16) -->      Y
 * ATTR_ARCH    (32) -->        X
 */

// fat_attr ---> inode->i_mode
u8 mode_from_fat_attr(const u8 fat_attr, const umode_t mode);
//inode->i_mode ---> fat_attr
int fat_attr_make_from_mode(const u8 mode, u8 * fat_attr);
#endif   /* FAT_MISC_H */
