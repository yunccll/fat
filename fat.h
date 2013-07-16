
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

#endif   /* FAT_H */
