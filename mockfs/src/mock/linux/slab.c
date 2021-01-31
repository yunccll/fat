#include "mock/linux/slab.h"

#include <stdlib.h>
void * kzalloc(size_t size, gfp_t flags){
	return calloc(size, 1);
}

void kfree(void *p){
	free(p);
}
