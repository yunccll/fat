
#ifndef  BASE_H
#define  BASE_H



#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include <stdio.h>

#include <linux/types.h>


#define BUGON(x)       assert(!(x))

#define TRUE  1
#define FALSE 0


#define pr_debug    printf
#define pr_info     printf
#define pr_error    printf


typedef __u64         sector_t;

#endif   /* BASE_H */
