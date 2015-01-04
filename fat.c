#include "fat.h"

#define Clus_Val_First(addr)    	((*(unsigned short int*)(addr)) & 0xfff)
#define Clus_Val_Second(addr)    	((*(unsigned short int*)(addr+1)) >>4)


int fat_cluster_val_with(uchar * fat_addr, size_t clusno)
{
	return clusno%2 == 0 ? Clus_Val_First(fat_addr+clusno/2*3) : Clus_Val_Second(fat_addr+clusno/2*3);
}

int fat_stat_info(uchar *fat_addr, size_t size)
{
    assert(fat_addr);

	int bad_cluster_cnt = 0;
	int free_cluster_cnt = 0;
	int used_cluster_cnt = 0;
	unsigned int val = 0;
	size_t i;
	for(i = 0; i < size*2/3; ++i)
	{	
		val = fat_cluster_val_with(fat_addr, i);
		if(val == NOT_USED_CLUSTER)
			++free_cluster_cnt;
		else if(val >= BAD_CLUSTER_NO_START && val <= BAD_CLUSTER_NO_END)
			++ bad_cluster_cnt;	
		else
			++ used_cluster_cnt;
	}
	FAT_DEBUG("total cluster : %d\n", size*2/3);
	FAT_DEBUG("sys cluster   : %d\n", 2);
	FAT_DEBUG("free cluster  : %d\n", free_cluster_cnt);
	FAT_DEBUG("bad cluster 	 : %d\n", bad_cluster_cnt);
	FAT_DEBUG("used cluster  : %d\n", used_cluster_cnt);
    return 0 ;
}

int fat_iterate_fat_clusno(uchar * fat_addr, size_t size, cb_iter_cluster_t cb_iter)
{
	size_t i;
	for(i = 0; i < size*2/3; ++i)
	{
		unsigned int val = fat_cluster_val_with(fat_addr, i);
		if(cb_iter)
		{
			if(cb_iter(i, val, NULL) != 0){
				FAT_ERROR("cb iter the clust[%d] error!\n", i);
			}
		}
	}
	return 0;
}
