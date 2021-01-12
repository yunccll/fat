#ifndef  BASE_H
#define  BASE_H



#include <assert.h>
#include <stdint.h>
#include <stddef.h>

#include <stdlib.h>
#include <stdio.h>

#include <string.h>


#include "types.h"
#include "err.h"
#include "unit_test_wrapper.h"


#define BUGON(x)       assert(!(x))



#define pr_debug    printf
#define pr_info     printf
#define pr_error    printf










#endif   /* BASE_H */
