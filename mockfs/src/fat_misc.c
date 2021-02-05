#include "fat_misc.h"
#include "mock/linux/time64.h"
#include "mock/uapi/linux/msdos_fs.h"
#include "mock/linux/stat.h"

#include "fat_super.h"

void fat_time_fat2unix(struct fat_sb *fsb, struct timespec64 *ts,
		__le16 __time, __le16 __date, u8 time_cs)
{

}

// fat_attr ---> inode->i_mode
u8 mode_from_fat_attr(const u8 fat_attr, const umode_t mode)
{
    umode_t ret_mode = mode;
    if(fat_attr & ATTR_RO){
        ret_mode &= ~S_IWUGO;
    }
    ret_mode |= ((fat_attr & ATTR_DIR) ? S_IFDIR : S_IFREG);
    return ret_mode; 
}
//inode->i_mode ---> fat_attr
int fat_attr_make_from_mode(const u8 mode, u8 * fat_attr)
{
    if(S_ISDIR(mode))
        *fat_attr |= ATTR_DIR;
     
    if( mode&S_IWUGO){ // writable 
        *fat_attr &= ~ATTR_RO;
    }
    else{ // readonly
        *fat_attr |= ATTR_RO;
    }
    return 0;
}
