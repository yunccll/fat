
#ifndef  FAT_H
#define  FAT_H


typedef struct fat_fat_s fat_fat_t;


struct fat_fat_s {
    int loop;
};


fat_fat_t * fat_fat_create();
void fat_fat_destroy(fat_fat_t * ptr);

void fat_fat_print(fat_fat_t * ptr);

int fat_fat_parser(fat_fat_t * ptr, const char* block_offset, int bytes);


#endif   /* FAT_H */
