#include "device.h"
#include "util.h"

#define INVALID_FD      (-1)

struct fat_dev_s {
    const char * _fname;
    int _fd;
    int _is_connected;
};


void fat_dev_destroy(fat_dev_t * ptr){
    if(ptr){
        if(0 != close(ptr->_fd)){
            ERROR_PRINT("close file [%s] error, error:%s\n", ptr->_fname, strerror(errno));
        }
        ptr->_is_connected = 0;
        ptr->_fname = NULL;
        ptr->_fd = INVALID_FD;
        free(ptr);
    }
}

fat_dev_t * fat_dev_create_file(const char * fname){
    IF_TRUE_RETURN(fname == NULL, NULL, "fname is NULL");

    fat_dev_t * ptr = (fat_dev_t*)calloc(sizeof(fat_dev_t), 1);
    IF_TRUE_RETURN(ptr == NULL, NULL, "No enough memory, will close the file\n");

    ptr->_fname = fname;
    ptr->_fd = open(fname, O_CREAT|O_RDWR,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
    IF_TRUE_GOTO(ptr->_fd < 0, error, "open file [%s] failed! err:%s\n", fname, strerror(errno));
    ptr->_is_connected = 1;
    return ptr;

error:
    free(ptr);
    return NULL;
}

int fat_dev_write(fat_dev_t * ptr, fat_offset_t offset,const uchar * block, int bytes){
    IF_TRUE_RETURN( ptr->_fd == INVALID_FD, -1, "open device first\n");

    int ret = lseek(ptr->_fd, offset, SEEK_SET);
    IF_TRUE_RETURN(ret < 0, ret, "lseek file failed! err:%s\n", strerror(errno));

    ret = write(ptr->_fd, block, bytes);
    IF_TRUE_RETURN(ret < 0, ret, "write file failed! err:%s\n", strerror(errno)); 

    return ret;
}

int fat_dev_read(fat_dev_t * ptr, fat_offset_t offset, uchar * block, int bytes){
    IF_TRUE_RETURN( ptr->_fd == INVALID_FD, -1, "open device first\n");

    int ret = lseek(ptr->_fd, offset, SEEK_SET);
    IF_TRUE_RETURN(ret < 0, ret, "lseek file failed! err:%s\n", strerror(errno));

    ret = read(ptr->_fd, block, bytes);
    IF_TRUE_RETURN(ret < 0,  ret, "read file failed! err:%s\n", strerror(errno)); 

    return ret;
}
