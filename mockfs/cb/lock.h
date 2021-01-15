
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
struct spin_lock_t {
};



//rwsem.h
struct rw_semaphore {
};

// mutex.h
struct mutex {
};


#endif   /* LOCK_H */
