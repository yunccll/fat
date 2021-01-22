
#ifndef  TYPES_H
#define  TYPES_H



#define TRUE  1
#define FALSE 0

#define true 1
#define false 0

typedef char __s8;
typedef short __s16;
typedef int __s32;
typedef long long __s64;

typedef unsigned char __u8;
typedef unsigned short __u16;
typedef unsigned int __u32;
typedef unsigned long long __u64;
//typedef int _Bool;

typedef __s8  s8;
typedef __u8  u8;
typedef __s16 s16;
typedef __u16 u16;
typedef __s32 s32;
typedef __u32 u32;
typedef __s64 s64;
typedef __u64 u64;

typedef _Bool bool;


#ifdef OS_X
typedef __u64         sector_t;
typedef __kernel_loff_t     loff_t;
typedef u64 blkcnt_t;
#else
#include <sys/types.h>
#endif

typedef long long   __kernel_loff_t;

typedef __s64 time64_t;
typedef __u64 timeu64_t;

typedef u64 sector_t;

typedef unsigned int fmode_t;


#endif   /* TYPES_H */
