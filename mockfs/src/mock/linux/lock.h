
#ifndef  LINUX_LOCK_H
#define  LINUX_LOCK_H

#include "mock/linux/compiler.h"
#include "mock/linux/types.h"

typedef struct {
    int counter;
} atomic_t;

typedef struct {
    s64 counter;
} atomic64_t;


typedef atomic64_t atomic_long_t;

#define ATOMIC_INIT(i)      { (i) }
#define ATOMIC64_INIT(i)    { (i) }

#define atomic_read(v)      READ_ONCE((v)->counter)
#define atomic64_read(v)    READ_ONCE((v)->counter)

#define atomic_set(v,i)     WRITE_ONCE(((v)->counter), (i))
#define atomic64_set(v,i)   WRITE_ONCE(((v)->counter), (i))

static inline void atomic_dec(atomic_t *v)
{
    atomic_set(v, atomic_read(v)-1);
}

/**
* atomic_dec_and_test - decrement and test
* @v: pointer of type atomic_t
*
* Atomically decrements @v by 1 and
* returns true if the result is 0, or false for all other
* cases.
*/
static inline bool atomic_dec_and_test(atomic_t *v)
{
    return (--v->counter) == 0 ? true : false;
}

/**
 * atomic_inc_not_zero - increment unless the number is zero
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1, if @v is non-zero.
 * Returns true if the increment was done.
 */
static inline bool atomic_inc_not_zero(atomic_t *v)
{
    if(v->counter != 0){
        ++v->counter;
        return true;
    }
    return false;
}




//spinlock_types.h
typedef struct  {
    volatile int count;
} spinlock_t;


#define __SPIN_LOCK_INITIALIZER(lockname) { .count = 0, }
#define __SPIN_LOCK_UNLOCKED(lockname)  (spinlock_t) __SPIN_LOCK_INITIALIZER(lockname)
#define DEFINE_SPINLOCK(x)    spinlock_t x = __SPIN_LOCK_UNLOCKED(x)

void spin_lock_init(spinlock_t * lock);
void spin_lock(spinlock_t * lock);
void spin_unlock(spinlock_t * lock);
// 1 - lock successfule, 0 - lock failed
int spin_trylock(spinlock_t *lock);
int spin_is_locked(spinlock_t *lock);

//void spin_lock_bh(spinlock_t *lock);
//void spin_lock_irq(spinlock_t *lock);
//void spin_lock_irqsave(spinlock_t * lock, unsigned long flags);

//void spin_lock_nested(spinlock_t * lock, int subclass);
//void spin_lock_irqsave_nested(spinlock_t * lock, unsigned long * flags, int subclass);
//void spin_lock_nest_lock(spinlock_t * lock, spinlock_t * nest_lock);


//void spin_unlock_bh(spinlock_t *lock);
//void spin_unlock_irq(spinlock_t *lock)
//void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags);

//int spin_trylock_bh(spinlock_t *lock);
//int spin_trylock_irq(spinlock_t *lock);
//int spin_trylock_irqsave(lock, flags);



struct semaphore {
    int count;
};

#define __SEMAPHORE_INITIALIZER(name, n)				\
{									\
	.count		= n,						\
}

#define DEFINE_SEMAPHORE(name)	\
	struct semaphore name = __SEMAPHORE_INITIALIZER(name, 1)

static inline void sema_init(struct semaphore *sem, int val)
{
	*sem = (struct semaphore) __SEMAPHORE_INITIALIZER(*sem, val);
}

// down() first -> and then  up()
extern void down(struct semaphore *sem);
extern int down_trylock(struct semaphore *sem);
extern void up(struct semaphore *sem);
//extern int down_interruptible(struct semaphore *sem);
//extern int down_killable(struct semaphore *sem);
//extern int down_timeout(struct semaphore *sem, long jiffies);



//rwsem.h  TODO: 
struct rw_semaphore {
    volatile int count;
    volatile int write_count;
};


/*  In all implementations count != 0 means locked */
static inline int rwsem_is_locked(struct rw_semaphore *sem)
{
    return sem->count != 0;
}

#define RWSEM_UNLOCKED_VALUE        0L


void init_rwsem(struct rw_semaphore *sem);

void down_read(struct rw_semaphore *sem);
/*
 * trylock for reading -- returns 1 if successful, 0 if contention
 */
int down_read_trylock(struct rw_semaphore *sem);

void down_write(struct rw_semaphore *sem);
/*
 * trylock for writing -- returns 1 if successful, 0 if contention
 */
int down_write_trylock(struct rw_semaphore *sem);
void up_read(struct rw_semaphore *sem);
void up_write(struct rw_semaphore *sem);

void downgrade_write(struct rw_semaphore *sem);
//int down_read_killable(struct rw_semaphore *sem);
//int down_write_killable(struct rw_semaphore *sem);




//TODO:
// mutex.h
struct mutex {
    volatile int count;
};

void mutex_init(struct mutex * lock);
void mutex_lock(struct mutex * lock);
void mutex_unlock(struct mutex * lock);
int mutex_trylock(struct mutex * lock);

//TODO: Next: finish the whole super_block related-function

#endif   /* LINUX_LOCK_H */
