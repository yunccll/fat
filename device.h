#ifndef  DEVICE_H
#define  DEVICE_H

#include "types.h"

typedef struct fat_dev_s fat_dev_t;

fat_dev_t * fat_dev_create_file(const char * fname);




void fat_dev_destroy(fat_dev_t * ptr);
fat_dev_t* fat_dev_create();


void fat_dev_uninit(fat_dev_t * ptr);
int fat_dev_init(fat_dev_t * ptr, const char * fname);


int fat_dev_write(fat_dev_t * ptr, fat_offset_t offset,const uchar * block, int bytes);

// return bytes to be read or -1 means err
int fat_dev_read(fat_dev_t * ptr, fat_offset_t offset, uchar * block, int bytes);


#endif   /* DEVICE_H */
