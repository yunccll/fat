#ifndef  DATA_H
#define  DATA_H

#include "common.h"

struct fat_data {
    fat_offset_t start_offset;
    int sectors_per_data;
    int bytes_per_sector;
    char *data_block;
};


typedef struct fat_data fat_data_t;


void fat_data_free(fat_data_t * pdata);
fat_data_t * fat_data_create();

void fat_data_deinit(fat_data_t * pdata);
int fat_data_init(fat_data_t * pdata, fat_offset_t start_offset, int sectors_per_data, int bytes_per_sector);


#endif   /* DATA_H */
