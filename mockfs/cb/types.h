
#ifndef  TYPES_H
#define  TYPES_H


#include <linux/types.h>

#define TRUE  1
#define FALSE 0

#define true 1
#define false 0

typedef __s8  s8;
typedef __u8  u8;
typedef __s16 s16;
typedef __u16 u16;
typedef __s32 s32;
typedef __u32 u32;
typedef __s64 s64;
typedef __u64 u64;

typedef _Bool bool;


typedef __u64         sector_t;
//typedef long long   __kernel_loff_t;
//typedef __kernel_loff_t     loff_t;

typedef __s64 time64_t;
typedef __u64 timeu64_t;


typedef unsigned int fmode_t;


#endif   /* TYPES_H */
