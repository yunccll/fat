#include "fat.h"

int offset_to_cluster_val(const uchar * buf, fat_offset_t offset)
{
    int ret = 0;
    switch(offset%3)
    {
        case 0:
            ret = ((buf[offset+1]&0x0f) << 8) + buf[offset];
            break;
        case 1:
            ret = (buf[offset] & 0x0f) + (buf[offset+1]<<4);
            break;
        case 2:
            ret = (buf[offset-1] & 0x0f) + (buf[offset] << 4);
             break;
        default:
           FAT_ERROR("fatal error!\n"); 
    }
    return ret;
}


int fat_stat_fat_info(uchar *buf, size_t size)
{
    assert(buf && size % 3 == 0);

	int bad_cluster_cnt = 0;
	int zero_cluster_cnt = 0;
	int used_cluster_cnt = 0;
	int val = 0;
	size_t i;
	for(i = 0; i < size%3*2; ++i)
	{	
		val = offset_to_cluster_val(buf, clusno_to_offset(i));
		if(val == NOT_USED_CLUSTER)
			++zero_cluster_cnt;
		else if(val >= BAD_CLUSTER_NO_START && val <= BAD_CLUSTER_NO_END)
			++ bad_cluster_cnt;	
		else
			++ used_cluster_cnt;
	}
	FAT_DEBUG("zero cluster : %d\n", zero_cluster_cnt);
	FAT_DEBUG("bad cluster 	: %d\n", zero_cluster_cnt);
	FAT_DEBUG("used cluster : %d\n", zero_cluster_cnt);
    return 0 ;
}
