#ifndef  MAKEFS_H
#define  MAKEFS_H

typedef struct fat_dev_s fat_dev_t;

int fat_make_fs(fat_dev_t * pdev);

#endif   /* MAKEFS_H */
