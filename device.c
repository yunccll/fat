#include "device.h"
#include "common.h"



static int open_file(fat_dev_t * pdev)
{
    assert(pdev);
    if( (pdev->fd = open(pdev->fname, pdev->flag))){
        FAT_ERROR("open file failed!\n"); 
        return -1;
    }
    return 0;
}
static void close_file(fat_dev_t * pdev)
{
    assert(pdev);
    close(pdev->fd);
    pdev->fd = INVALID_FD;
}



void fat_dev_destroy(fat_dev_t * pdev)
{
    assert(pdev);
    if(pdev->fd != INVALID_FD){
        close_file(pdev);
    }
}
void fat_dev_init(fat_dev_t * pdev, const char * fname, unsigned int flag)
{
    assert(pdev && fname);
    pdev->fname = fname;
    pdev->fd = INVALID_FD;
    pdev->flag = flag;
}

int fat_dev_read(fat_dev_t * pdev, int offset, char * buf, int sz)
{
    assert(pdev);
    if(pdev->fd == INVALID_FD){
        if(open_file(pdev)) {
            FAT_ERROR("open file failed!\n");
            return -1;
        }
    }

    int ret = lseek(pdev->fd, offset, SEEK_SET);
    if(ret < 0){
        FAT_ERROR("lseek file failed!\n");
        return ret;
    }

    ret = read(pdev->fd, buf, sz);
    if(ret < 0){
        FAT_ERROR("read file failed!\n"); 
        return ret;
    }
    return ret;
}
int fat_dev_write(fat_dev_t * pdev, int offset, const char * buf, int sz)
{
    assert(pdev);
    if(pdev->fd == INVALID_FD){
        if(open_file(pdev)) {
            FAT_ERROR("open file failed!\n");
            return -1;
        }
    }

    int ret = lseek(pdev->fd, offset, SEEK_SET);
    if(ret < 0){
        FAT_ERROR("lseek file failed!\n");
        return ret;
    }

    ret = write(pdev->fd, buf, sz);
    if(ret < 0){
        FAT_ERROR("read file failed!\n"); 
        return ret;
    }
    return ret;
}
