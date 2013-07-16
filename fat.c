#include "fat.h"

int offset_to_cluster_val(const uchar * buf, fat_offset_t offset)
{
    int ret = 0;
    switch(offset%3)
    {
        case 0:
            ret = ((buf[offset+1]&0xf0) << 4) + buf[offset];
            break;
        case 1:
            ret = (buf[offset] & 0xf0) + (buf[offset+1] << 4);
            break;
        case 2:
            ret = (buf[offset-1] & 0xf0) + (buf[offset] << 4);
             break;
        default:
           FAT_ERROR("fatal error!\n"); 
    }
    return ret;
}
