#include "boot.h"

#include "util.h"

struct fat_boot_s {
    int loop;
};

fat_boot_t * fat_boot_create(){
    return NULL;
}

void fat_boot_destroy(fat_boot_t * ptr){
    if(ptr){
        free(ptr);
    }
}


