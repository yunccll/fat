#ifndef  DEVICE_H
#define  DEVICE_H

#include "types.h"

typedef struct fat_dev_s fat_dev_t;


fat_dev_t * fat_dev_create_file(const char * fname);
void fat_dev_destroy(fat_dev_t * ptr);

int fat_dev_write(fat_dev_t * ptr, fat_offset_t offset,const char * block, int bytes);

// return bytes to be read or -1 means err
int fat_dev_read(fat_dev_t * ptr, fat_offset_t offset, char * block, int bytes);

int fat_dev_is_open(fat_dev_t * ptr);
const char * fat_dev_fname(fat_dev_t * ptr);


#endif   /* DEVICE_H */
