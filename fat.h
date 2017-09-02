
#ifndef  FAT_H
#define  FAT_H

#include "common.h"


// byte offset(0-based) to cluster no(0-based)
#define offset_to_clusno(offset)   ( ((offset)%3) ? ((offset)/3*2+1) : ((offset)/3*2) )
#define clusno_to_offset(clusno)     ((clusno) *3/2)

// cluster 0,1 is not used
// cluster no(0-based) to  physical sector no(0-based)
#define clusno_to_sectno(clusno)     ((clusno) + 31)
#define sectno_to_clusno(sectno)     ((sectno) - 31)


// format fat buffer
//TODO????

// iterate the fat buffer : TODO????
typedef int (*cb_iter_cluster_t)(size_t clusno, unsigned int val, void * data);
int fat_iterate_fat_clusno(uchar * fat_addr, size_t size, cb_iter_cluster_t cb_iter);

// static all clust info
int fat_stat_info(uchar *fat_addr, size_t size);
int fat_cluster_val_with(uchar * fat_addr, size_t clusno);


#define BAD_CLUSTER_NO_START		0xFF0
#define BAD_CLUSTER_NO_END			0xFF7

#define LAST_CLUSTER_NO_START		0xFF8
#define LAST_CLUSTER_NO_END			0xFFF

#define NOT_USED_CLUSTER			0x000
#endif   /* FAT_H */
