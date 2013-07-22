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

int fat_iterate_buffer(uchar * buf, size_t size)
{
    return 0;
}

int fat_print_fat_next_clusno(uchar * buf, size_t size)
{
    assert(buf && size % 3 == 0);
    return 0 ;
}
