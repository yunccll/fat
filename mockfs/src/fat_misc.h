
#ifndef  FAT_MISC_H
#define  FAT_MISC_H

#include "mock/linux/types.h"

struct fat_sb;
struct timespec64;

void fat_time_fat2unix(struct fat_sb *fsb, struct timespec64 *ts,
		__le16 __time, __le16 __date, u8 time_cs);


#endif   /* FAT_MISC_H */
