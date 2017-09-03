#include "data.h"

void fat_data_free(fat_data_t * pdata){
    if(pdata){
        free(pdata);
    }
}
fat_data_t* fat_data_create(){
    return (fat_data_t*)calloc(sizeof(fat_data_t), 1);
}

void fat_data_deinit(fat_data_t * pdata){
    if(pdata){
        if(pdata->data_block){
            free(pdata->data_block);
            pdata->data_block = NULL;
        }
    }
}
int fat_data_init(fat_data_t * pdata, fat_offset_t start_offset, int sectors_per_data, int bytes_per_sector){
    if(pdata){
        pdata->start_offset = start_offset;
        pdata->sectors_per_data = sectors_per_data;
        pdata->bytes_per_sector = bytes_per_sector;
        pdata->data_block = (char*)calloc(sizeof(char)*bytes_per_sector, 1);
        return (pdata->data_block == NULL)  ?  -1 : 0;
    }
    return -1;
}
