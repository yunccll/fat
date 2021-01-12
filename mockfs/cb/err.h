#ifndef  ERR_H
#define  ERR_H

#include "errno.h"

/* err: [-4095, 0】
* void * ptr =createNode();
* if(IS_ERR(ptr)){
*   return PTR_ERR(ptr);
* }
*/

#define MAX_ERRNO   4095
#define PTR_TO_LONG(x)  ((long)(void *)(x))
#define IS_ERR_VALUE(x) (PTR_TO_LONG(x) <= 0 && PTR_TO_LONG(x) >= -MAX_ERRNO)

static inline bool IS_ERR(const void * ptr)
{
    return IS_ERR_VALUE(ptr);
}
static inline bool IS_ERR_OR_NULL(const void * ptr)
{
    return (!ptr)||IS_ERR_VALUE(ptr);
}

static inline long PTR_ERR(const void * ptr)
{
    return (long)ptr;
}

static inline void * ERR_PTR(long error)
{
    return (void *) error;
}

static inline void * ERR_CAST( const void *ptr)
{
    return (void *) ptr;
}

static inline int PTR_ERR_OR_ZERO( const void *ptr)
{
    if (IS_ERR(ptr))
        return PTR_ERR(ptr);
    else
        return 0;
}

#endif   /* ERR_H */
