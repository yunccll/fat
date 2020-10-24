
#ifndef  BASE_H
#define  BASE_H



#include <cassert>
#include <cstdint>
#include <cstddef>

#include <linux/types.h>


#define BUGON(x)       assert(!(x))

#define TRUE  1
#define FALSE 0



typedef __u64         sector_t;

#endif   /* BASE_H */
