#include "fat.h"

#include "util.h"

fat_fat_t * fat_fat_create(){
    fat_fat_t * ptr = (fat_fat_t*)calloc(sizeof(fat_fat_t), 1);
    IF_TRUE_RETURN(ptr == NULL, NULL, "no enough memory\n");
    return ptr;
}
void fat_fat_destroy(fat_fat_t * ptr){
    if(ptr){
        free(ptr);
    }
}

void fat_fat_print(fat_fat_t * ptr){

}

int fat_fat_parser(fat_fat_t * ptr, const char * block_offset, int bytes){
    return 0;
}
