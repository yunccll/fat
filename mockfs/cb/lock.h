
#ifndef  LOCK_H
#define  LOCK_H

#include "compiler.h"
#include "types.h"

typedef struct {
    int counter;
} atomic_t;

typedef struct {
        s64 counter;
} atomic64_t;

#define ATOMIC_INIT(i)      { (i) }
#define ATOMIC64_INIT(i)    { (i) }

#define atomic_read(v)      READ_ONCE((v)->counter)
#define atomic64_read(v)    READ_ONCE((v)->counter)

#define atomic_set(v,i)     WRITE_ONCE(((v)->counter), (i))
#define atomic64_set(v,i)   WRITE_ONCE(((v)->counter), (i))






struct rw_semaphore {
};


#endif   /* LOCK_H */
