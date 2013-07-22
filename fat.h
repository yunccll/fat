
#ifndef  FAT_H
#define  FAT_H

#include "common.h"


// byte offset(0-based) to cluster no(0-based)
#define offset_to_clusno(offset)   ( ((offset)%3) ? ((offset)/3*2+1) : ((offset)/3*2) )
#define clusno_to_offset(clusno)     ((clusno) *3/2)

// get the cluster val with the byte offset
int offset_to_cluster_val(const uchar * buf, fat_offset_t offset);

// cluster no(0-based) to  physical sector no(0-based)
#define clusno_to_sectno(clusno)     ((clusno) + 31)
#define sectno_to_clusno(sectno)     ((sectno) - 31)


// format fat buffer
// iterate the fat buffer
typedef int (*cb_iter_cluster_t)(void * data);
int fat_iterate_fat_clusno(uchar * buf, size_t size, cb_iter_cluster_t cb_iter);

int fat_print_fat_next_clusno(uchar *buf, size_t size);

#endif   /* FAT_H */
