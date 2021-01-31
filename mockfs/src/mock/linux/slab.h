#ifndef  LINUX_SLAB_H
#define  LINUX_SLAB_H


#include "mock/linux/types.h"
#include "mock/linux/gfp.h"

void * kzalloc(size_t size, gfp_t flags);
void kfree(void *p);


#endif   /* LINUX_SLAB_H */
