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



fat_fat_t * fat_fat_create(){
    return (fat_fat_t*)malloc(sizeof(fat_fat_t));
}
void fat_fat_free(fat_fat_t * pfat){
    if(pfat != NULL) free(pfat);
}

static void __free_fats_origin_memory(char ** fats, int number){
    int i = 0;
    for(; i < number; ++i){
        if(fats[i] != NULL){
            free(fats[i]);
            fats[i] = NULL;
        }
    }
}
int fat_fat_init(fat_fat_t * pfat, fat_offset_t start_offset, int number_of_fat, int sectors_per_fat, int bytes_per_sector){
    if(pfat == NULL) return -1; 

    pfat->start_offset = start_offset;
    pfat->number_of_fat = number_of_fat;
    pfat->sectors_per_fat = sectors_per_fat;
    pfat->bytes_per_sector = bytes_per_sector;


    pfat->fats = (char**)calloc(sizeof(char*), pfat->number_of_fat);
    if(pfat->fats == NULL){
        FAT_ERROR("malloc the fats[2] array failed!\n");
        return -1;
    }
    int i = 0; 
    for(; i < pfat->number_of_fat; ++i){
        pfat->fats[i] = (char*)calloc(sizeof(char)*pfat->sectors_per_fat * pfat->bytes_per_sector, 1);
        if(pfat->fats[i] == NULL){
            FAT_ERROR("malloc the fat[%d] failed\n", i+1);
            break;
        }
    }
    if(i == pfat->number_of_fat){
        return 0;
    }
    __free_fats_origin_memory(pfat->fats, pfat->number_of_fat);
    return -1;
}

void fat_fat_destroy(fat_fat_t * pfat){
    if(pfat != NULL){
        __free_fats_origin_memory(pfat->fats, pfat->number_of_fat);
    }
}
