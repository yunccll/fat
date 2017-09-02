
#ifndef  COMMON_H
#define  COMMON_H

#include <assert.h>
#include "log.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>


#include <linux/types.h>
#include <fcntl.h>
#include <unistd.h>

typedef unsigned char uchar;
typedef unsigned int fat_offset_t;


#define  two_bytes_to_short(bytes)       (*(short*)(bytes))

#define  two_bytes_to_int(bytes)       (*(unsigned short*)(bytes))
#define  four_bytes_to_int(bytes)      (*(unsigned int *)(bytes))

#define INVALID_FD      -1


#endif   /* COMMON_H */
