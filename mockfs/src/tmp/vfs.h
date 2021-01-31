
#ifndef  VFS_H
#define  VFS_H



struct vfs {
    void * file_system;
};

struct vfs * vfs_create(void);
void vfs_destroy(struct vfs * vfs);


#endif   /* VFS_H */
