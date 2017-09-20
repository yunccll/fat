#include "device.h"
#include "includes.h"

#define INVALID_FD      (-1)

struct fat_dev_s {
    const char * _fname;
    int _fd;
};

static int __open_dev(const char * fname, int flag){
    return open(fname, flag);
}
static int __close_dev(int fd){
    return close(fd);
}

void fat_dev_destroy(fat_dev_t * ptr){
    if(ptr){
        free(ptr);
    }
}
fat_dev_t* fat_dev_create(){
    return (fat_dev_t*)calloc(sizeof(fat_dev_t), 1);
}

void fat_dev_uninit(fat_dev_t * ptr){
    if(ptr){
        if( 0 != __close_dev(ptr->_fd)){
            printf("close file [%s] error, error:%s\n", ptr->_fname, strerror(errno));
            return ;
        }
        ptr->_fname = NULL;
        ptr->_fd = INVALID_FD;
    }
}


int fat_dev_init(fat_dev_t * ptr, const char * fname){
    if(ptr){
        if(fname == NULL){
            printf("file name is NULL!\n");
            return -1;
        }
        ptr->_fname = fname;
        ptr->_fd = __open_dev(fname, O_RDWR);
        if(ptr->_fd == -1){
            printf("open file %s failed! err:%s\n", fname, strerror(errno));
            return -1;
        }
    }
    return 0;
}

int fat_dev_write(fat_dev_t * ptr, fat_offset_t offset,const uchar * block, int bytes){
    if(ptr->_fd == INVALID_FD){
        printf("open device first\n");
        return -1;
    }

    int ret = lseek(ptr->_fd, offset, SEEK_SET);
    if(ret < 0){
        printf("lseek file failed! err:%s\n", strerror(errno));
        return ret;
    }

    ret = write(ptr->_fd, block, bytes);
    if(ret < 0){
        printf("write file failed! err:%s\n", strerror(errno)); 
        return ret;
    }
    return ret;
}

// return bytes to be read or -1 means err
int fat_dev_read(fat_dev_t * ptr, fat_offset_t offset, uchar * block, int bytes){
    if(ptr->_fd == INVALID_FD){
        printf("open device first\n");
        return -1;
    }

    int ret = lseek(ptr->_fd, offset, SEEK_SET);
    if(ret < 0){
        printf("lseek file failed! err:%s\n", strerror(errno));
        return ret;
    }

    ret = read(ptr->_fd, block, bytes);
    if(ret < 0){
        printf("read file failed! err:%s\n", strerror(errno)); 
        return ret;
    }
    return ret;
}

fat_dev_t * fat_dev_create_file(const char * fname){
    if(fname == NULL){ 
        printf("fname is NULL");
        return NULL;
    }
    int fd = open(fname, O_CREAT|O_RDWR,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
    if(fd < 0){
        printf("open file [%s] failed! err:%s\n", fname, strerror(errno));
        return NULL;
    }

    fat_dev_t * ptr = fat_dev_create();
    if(ptr == NULL){
        printf("No enough memory, will close the file\n");
        goto close_file;
    }
    ptr->_fname = fname;
    ptr->_fd = fd;
    return ptr;

close_file:
    close(fd);
    unlink(fname);
    return NULL;
}
