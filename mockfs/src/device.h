#ifndef  DEVICE_H
#define  DEVICE_H

#include "mock/linux/types.h"

typedef struct mcfs_dev mcfs_dev_t;


mcfs_dev_t * mcfs_dev_create_file(const char * fname);
void mcfs_dev_destroy(mcfs_dev_t * ptr);

long long mcfs_dev_write(mcfs_dev_t * ptr, loff_t offset,const char * block, long long bytes);

// return bytes to be read or -1 means err
long long mcfs_dev_read(mcfs_dev_t * ptr, loff_t offset, char * block, long long bytes);

int mcfs_dev_is_open(mcfs_dev_t * ptr);
const char * mcfs_dev_fname(mcfs_dev_t * ptr);


void mcfs_dev_print(mcfs_dev_t * ptr);


#endif   /* DEVICE_H */
