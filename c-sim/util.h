
#ifndef  UTIL_H
#define  UTIL_H


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#include "types.h"

//#define NO_LOG  1
#ifdef NO_LOG   
    #define     TRACE_PRINT(fmt, ...)   printf(fmt, ##__VA_ARGS__)
    #define     DEBUG_PRINT(fmt, ...)
    #define     ERROR_PRINT(fmt, ...)   printf(fmt, ##__VA_ARGS__)
#else
    #define     TRACE_PRINT(fmt, ...)   printf(fmt, ##__VA_ARGS__)
    #define     DEBUG_PRINT(fmt, ...)   printf(fmt, ##__VA_ARGS__)
    #define     ERROR_PRINT(fmt, ...)   printf(fmt, ##__VA_ARGS__)
#endif

#define IF_TRUE_RETURN_VOID(cond, fmt, ...)  do {\
    if((cond)) { \
        ERROR_PRINT(fmt, ##__VA_ARGS__); \
        return ; \
    } \
} while (0)

#define IF_TRUE_RETURN(cond, ret, fmt, ...) do { \
    if((cond)) { \
        ERROR_PRINT(fmt, ##__VA_ARGS__); \
        return ret; \
    } \
} while (0)

#define IF_TRUE_GOTO(cond, label, fmt, ...) do { \
    if((cond)) { \
        ERROR_PRINT(fmt, ##__VA_ARGS__); \
        goto label; \
    } \
} while (0)

#endif   /* UTIL_H */
