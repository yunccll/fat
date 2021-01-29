#include "fat_super.h"
#include "base.h"

void fat_super_block_free(struct fat_super_block * sbf)
{
    free(sbf);
}
struct fat_super_block * fat_super_block_create(u8 * data, unsigned long size)
{
    if(!data){
        return NULL;
    }

    struct fat_super_block * ptr = (struct fat_super_block*)calloc(sizeof(struct fat_super_block), 1);
    //TODO:
    pr_debug("MRB info %x,%x,%x\n", data[0], data[1], data[2]);
    return ptr;
}
