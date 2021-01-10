#ifndef  BASE_H
#define  BASE_H



#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#include <linux/types.h>


#include "unit_test_wrapper.h"


#define BUGON(x)       assert(!(x))

#define TRUE  1
#define FALSE 0


#define pr_debug    printf
#define pr_info     printf
#define pr_error    printf


typedef __u64         sector_t;




#define EBUSY	500
#define EINVAL	501


#endif   /* BASE_H */
