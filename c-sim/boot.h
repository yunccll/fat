
#ifndef  BOOT_H
#define  BOOT_H


typedef struct fat_boot_s fat_boot_t;

fat_boot_t * fat_boot_create();
void fat_boot_destroy(fat_boot_t * ptr);





#endif   /* BOOT_H */
