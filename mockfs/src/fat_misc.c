#include "fat_misc.h"
#include "mock/linux/time64.h"

#include "fat_super.h"

void fat_time_fat2unix(struct fat_sb *fsb, struct timespec64 *ts,
		__le16 __time, __le16 __date, u8 time_cs)
{

}
