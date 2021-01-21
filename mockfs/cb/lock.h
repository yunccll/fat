
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


typedef atomic64_t atomic_long_t;

#define ATOMIC_INIT(i)      { (i) }
#define ATOMIC64_INIT(i)    { (i) }

#define atomic_read(v)      READ_ONCE((v)->counter)
#define atomic64_read(v)    READ_ONCE((v)->counter)

#define atomic_set(v,i)     WRITE_ONCE(((v)->counter), (i))
#define atomic64_set(v,i)   WRITE_ONCE(((v)->counter), (i))


/* This counts to 12. Any more, it will return 13th argument. */
#define __RMWcc_ARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _n, X...) _n
#define RMWcc_ARGS(X...) __RMWcc_ARGS(, ##X, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define __RMWcc_CONCAT(a, b) a ## b
#define RMWcc_CONCAT(a, b) __RMWcc_CONCAT(a, b)

#define __CLOBBERS_MEM(clb...)	"memory", ## clb

#define asm_volatile_goto(x...)	do { asm goto(x); asm (""); } while (0)

#define __GEN_RMWcc(fullop, _var, cc, clobbers, ...)			\
({									\
	bool c = false;							\
	asm_volatile_goto (fullop "; j" #cc " %l[cc_label]"		\
			: : [var] "m" (_var), ## __VA_ARGS__		\
			: clobbers : cc_label);				\
	if (0) {							\
cc_label:	c = true;						\
	}								\
	c;								\
})


#define GEN_UNARY_RMWcc_4(op, var, cc, arg0)				\
	__GEN_RMWcc(op " " arg0, var, cc, __CLOBBERS_MEM())

#define GEN_UNARY_RMWcc_3(op, var, cc)					\
	GEN_UNARY_RMWcc_4(op, var, cc, "%[var]")

#define GEN_UNARY_RMWcc(X...) RMWcc_CONCAT(GEN_UNARY_RMWcc_, RMWcc_ARGS(X))(X)

#define GEN_BINARY_RMWcc_6(op, var, cc, vcon, _val, arg0)		\
	__GEN_RMWcc(op " %[val], " arg0, var, cc,			\
		    __CLOBBERS_MEM(), [val] vcon (_val))

#define GEN_BINARY_RMWcc_5(op, var, cc, vcon, val)			\
	GEN_BINARY_RMWcc_6(op, var, cc, vcon, val, "%[var]")

#define GEN_BINARY_RMWcc(X...) RMWcc_CONCAT(GEN_BINARY_RMWcc_, RMWcc_ARGS(X))(X)

#define GEN_UNARY_SUFFIXED_RMWcc(op, suffix, var, cc, clobbers...)	\
	__GEN_RMWcc(op " %[var]\n\t" suffix, var, cc,			\
		    __CLOBBERS_MEM(clobbers))

#define GEN_BINARY_SUFFIXED_RMWcc(op, suffix, var, cc, vcon, _val, clobbers...)\
	__GEN_RMWcc(op " %[val], %[var]\n\t" suffix, var, cc,		\
		    __CLOBBERS_MEM(clobbers), [val] vcon (_val))

#define LOCK_PREFIX_HERE \
		".pushsection .smp_locks,\"a\"\n"	\
		".balign 4\n"				\
		".long 671f - .\n" /* offset */		\
		".popsection\n"				\
		"671:"
#define LOCK_PREFIX LOCK_PREFIX_HERE "\n\tlock; "

static __always_inline bool arch_atomic_dec_and_test(atomic_t *v)
{
	return GEN_UNARY_RMWcc(LOCK_PREFIX "decl", v->counter, e);
}
static inline bool atomic_dec_and_test(atomic_t *v)
{
	return arch_atomic_dec_and_test(v);
}




//spinlock_types.h
typedef struct  {
    volatile int count;
} spinlock_t;

void spin_lock_init(spinlock_t * lock);
void spin_lock(spinlock_t * lock);
void spin_unlock(spinlock_t * lock);
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



//rwsem.h
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

#endif   /* LOCK_H */
