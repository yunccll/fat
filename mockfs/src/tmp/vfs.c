#include "vfs.h"
#include "base.h"

struct vfs * vfs_create(void){
    struct vfs * ptr = (struct vfs*) calloc(sizeof(struct vfs), 1);
    ptr->file_system = NULL;
    return ptr;
}
void vfs_destroy(struct vfs * vfs){
    free(vfs);
}
