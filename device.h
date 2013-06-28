
#ifndef  DEVICE_H
#define  DEVICE_H



struct fat_dev {
    const char * fname;
    int fd;
    unsigned int  flag;
};
typedef struct fat_dev fat_dev_t;


void fat_dev_init(fat_dev_t * pdev, const char * fname, unsigned int flag);
int fat_dev_read(fat_dev_t * pdev, int offset, char * buf, int sz);
int fat_dev_write(fat_dev_t * pdev, int offset, const char * buf, int sz);
void fat_dev_destroy(fat_dev_t * pdev);

#endif   /* DEVICE_H */
