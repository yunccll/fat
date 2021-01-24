#include "device.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#define     TRACE_PRINT(fmt, ...)   printf(fmt, ##__VA_ARGS__)
#define     DEBUG_PRINT(fmt, ...)   printf(fmt, ##__VA_ARGS__)
#define     ERROR_PRINT(fmt, ...)   printf(fmt, ##__VA_ARGS__)

#define INVALID_FD      (-1)
#define IF_TRUE_RETURN_VOID(cond, fmt, ...)  do {\
    if((cond)) { \
        ERROR_PRINT(fmt, ##__VA_ARGS__); \
        return ; \
    } \
} while (0)

#define IF_TRUE_RETURN(cond, ret, fmt, ...) do { \
    if((cond)) { \
        ERROR_PRINT(fmt, ##__VA_ARGS__); \
        return ret; \
    } \
} while (0)

#define IF_TRUE_GOTO(cond, label, fmt, ...) do { \
    if((cond)) { \
        ERROR_PRINT(fmt, ##__VA_ARGS__); \
        goto label; \
    } \
} while (0)

struct mcfs_dev {
    const char * _fname;
    int _fd;
    int _is_connected;
};


void mcfs_dev_destroy(mcfs_dev_t * ptr){
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

mcfs_dev_t * mcfs_dev_create_file(const char * fname){
    IF_TRUE_RETURN(fname == NULL, NULL, "fname is NULL");

    mcfs_dev_t * ptr = (mcfs_dev_t*)calloc(sizeof(mcfs_dev_t), 1);
    IF_TRUE_RETURN(ptr == NULL, NULL, "No enough memory, will close the file\n");

    ptr->_fname = fname;
    ptr->_fd = open(fname, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
    IF_TRUE_GOTO(ptr->_fd < 0, error, "open file [%s] failed! err:%s\n", fname, strerror(errno));
    ptr->_is_connected = 1;
    return ptr;

error:
    free(ptr);
    return NULL;
}

long long mcfs_dev_write(mcfs_dev_t * ptr, loff_t offset,const char * block, long long bytes){
    IF_TRUE_RETURN( ptr->_fd == INVALID_FD, -1, "open device first\n");

    long long ret = lseek(ptr->_fd, offset, SEEK_SET);
    IF_TRUE_RETURN(ret < 0, ret, "lseek file failed! err:%s\n", strerror(errno));

    ret = write(ptr->_fd, block, bytes);
    IF_TRUE_RETURN(ret < 0, ret, "write file failed! err:%s\n", strerror(errno)); 

    return ret;
}

long long  mcfs_dev_read(mcfs_dev_t * ptr, loff_t offset, char * block, long long bytes){
    IF_TRUE_RETURN( ptr->_fd == INVALID_FD, -1, "open device first\n");

    long long ret = lseek(ptr->_fd, offset, SEEK_SET);
    IF_TRUE_RETURN(ret < 0, ret, "lseek file failed! err:%s\n", strerror(errno));
        
    //TRACE_PRINT("fd:%d, is_connected:%d, fname:%s, block:%p, bytes:%llu\n", ptr->_fd, ptr->_is_connected, ptr->_fname, block, bytes);
    ret = read(ptr->_fd, block, bytes);
    IF_TRUE_RETURN(ret < 0,  ret, "read file failed! err:%s\n", strerror(errno)); 
    return ret;
}

int mcfs_dev_is_open(mcfs_dev_t * ptr){
    return ptr->_is_connected == 1;
}
const char * mcfs_dev_fname(mcfs_dev_t * ptr){
    return ptr->_fname;
}

void mcfs_dev_print(mcfs_dev_t * ptr){
    TRACE_PRINT("fd:%d, is_connected:%d, name:%s\n", ptr->_fd, ptr->_is_connected, ptr->_fname);
}
