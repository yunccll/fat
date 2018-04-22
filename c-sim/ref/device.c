#include "device.h"


static int open_file(fat_dev_t * pdev)
{
    assert(pdev);
    if( (pdev->fd = open(pdev->fname, pdev->flag)) < 0){
        FAT_ERROR("open file %s failed! err:%s\n", pdev->fname, strerror(errno)); 
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



static int create_file(fat_dev_t * pdev, mode_t mode)
{
    assert(pdev);
    if( (pdev->fd = open(pdev->fname, pdev->flag, mode)) < 0){
        FAT_ERROR("open file failed! err:%s\n", strerror(errno));
        return -1;
    }
    return 0;
}
int fat_dev_create_file(fat_dev_t * pdev, const char * fname)
{
    assert(pdev && fname);
    fat_dev_init(pdev, fname, O_CREAT|O_RDWR);
    if( 0 > create_file(pdev, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH)){
        FAT_ERROR("create file failed! err:%s\n", strerror(errno));
        return -1;
    }
    return 0;
}
int fat_dev_read(fat_dev_t * pdev, fat_offset_t offset, uchar * buf, int sz)
{
    assert(pdev && buf && sz >= 0 && offset >= 0);
    if(pdev->fd == INVALID_FD){
        if(open_file(pdev)) {
            FAT_ERROR("call open file failed!");
            return -1;
        }
    }

    int ret = lseek(pdev->fd, offset, SEEK_SET);
    if(ret < 0){
        FAT_ERROR("lseek file failed! err:%s\n", strerror(errno));
        return ret;
    }

    ret = read(pdev->fd, buf, sz);
    if(ret < 0){
        FAT_ERROR("read file failed! err:%s\n", strerror(errno)); 
        return ret;
    }
    return ret;
}
int fat_dev_write(fat_dev_t * pdev, fat_offset_t offset, const uchar * buf, int sz)
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
        FAT_ERROR("lseek file failed! err:%s\n", strerror(errno));
        return ret;
    }

    ret = write(pdev->fd, buf, sz);
    if(ret < 0){
        FAT_ERROR("write file failed! err:%s\n", strerror(errno)); 
        return ret;
    }
    return ret;
}
