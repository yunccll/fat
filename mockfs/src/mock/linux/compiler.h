
#ifndef  LINUX_COMPILER_H
#define  LINUX_COMPILER_H

#include "mock/linux/types.h"


#define smp_mb()    barrier()


#define ___PASTE(a,b) a##b
#define __PASTE(a,b) ___PASTE(a,b)

/* Not-quite-unique ID. */
#ifndef __UNIQUE_ID
# define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __LINE__)
#endif



#define barrier() __asm__ __volatile__("": : :"memory")

#define read_barrier_depends()		do { } while (0)
#define __smp_read_barrier_depends()	read_barrier_depends()
#define smp_read_barrier_depends()	__smp_read_barrier_depends()

#define __READ_ONCE_SIZE						\
({									\
	switch (size) {							\
	case 1: *(__u8 *)res = *(volatile __u8 *)p; break;		\
	case 2: *(__u16 *)res = *(volatile __u16 *)p; break;		\
	case 4: *(__u32 *)res = *(volatile __u32 *)p; break;		\
	case 8: *(__u64 *)res = *(volatile __u64 *)p; break;		\
	default:							\
		barrier();						\
		__builtin_memcpy((void *)res, (const void *)p, size);	\
		barrier();						\
	}								\
})


static inline void __read_once_size(const volatile void *p, void *res, int size)
{
	__READ_ONCE_SIZE;
}

static inline void __read_once_size_nocheck(const volatile void *p, void *res, int size)
{
	__READ_ONCE_SIZE;
}

#define __READ_ONCE(x, check)						\
({									\
	union { typeof(x) __val; char __c[1]; } __u;			\
	if (check)							\
		__read_once_size(&(x), __u.__c, sizeof(x));		\
	else								\
		__read_once_size_nocheck(&(x), __u.__c, sizeof(x));	\
	smp_read_barrier_depends(); /* Enforce dependency ordering from x */ \
	__u.__val;							\
})
#define READ_ONCE(x) __READ_ONCE(x, 1)



static inline void __write_once_size(volatile void *p, void *res, int size)
{
	switch (size) {
	case 1: *(volatile __u8 *)p = *(__u8 *)res; break;
	case 2: *(volatile __u16 *)p = *(__u16 *)res; break;
	case 4: *(volatile __u32 *)p = *(__u32 *)res; break;
	case 8: *(volatile __u64 *)p = *(__u64 *)res; break;
	default:
		barrier();
		__builtin_memcpy((void *)p, (const void *)res, size);
		barrier();
	}
}

#define WRITE_ONCE(x, val) \
({							\
	union { typeof(x) __val; char __c[1]; } __u =	\
		{ .__val = ( typeof(x)) (val) }; \
	__write_once_size(&(x), __u.__c, sizeof(x));	\
	__u.__val;					\
})


# ifndef likely
#  define likely(x) (x)
# endif
# ifndef unlikely
#  define unlikely(x)   (x)
# endif



#endif   /* LINUX_COMPILER_H */
