
#ifndef  COMMON_H
#define  COMMON_H

#include <assert.h>
#include "log.h"

#define INVALID_FD      -1

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <linux/types.h>


typedef unsigned char uchar;
typedef unsigned int fat_offset_t;


#define  two_bytes_to_int(bytes)       (((int) (*( (bytes)+1 )) << 8 ) + *(bytes))
#define  four_bytes_to_int(bytes)      ( ((*(bytes)+3) << 24)+((*(bytes)+2) << 16)+(*(bytes+1) << 8)+(*(bytes))  )

#endif   /* COMMON_H */
