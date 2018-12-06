
#ifndef  FS_H
#define  FS_H

typedef struct fat_fs_s fat_fs_t;


void fat_fs_destroy(fat_fs_t * fs);
fat_fs_t * fat_fs_create(const char * dev_name);
int fat_fs_print(fat_fs_t * fs);

int fat_fs_load_data(fat_fs_t * ptr);


#endif   /* FS_H */
