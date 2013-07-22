
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


#define  two_bytes_to_int(bytes)       (((int) (*( (bytes)+1 )) << 8 ) + *(bytes))
#define  four_bytes_to_int(bytes)      ( ((*(bytes)+3) << 24)+((*(bytes)+2) << 16)+(*(bytes+1) << 8)+(*(bytes))  )

#define INVALID_FD      -1


#endif   /* COMMON_H */
