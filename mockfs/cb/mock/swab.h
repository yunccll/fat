/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
#ifndef SWAB_H
#define SWAB_H

#include "../types.h"

#define __BITS_PER_LONG  64

//#include <linux/types.h>
//#include <linux/compiler.h>
//#include <asm/bitsperlong.h>
//#include <asm/swab.h>

/*
 * casts are necessary for constants, because we never know how for sure
 * how U/UL/ULL map to __u16, __u32, __u64. At least not in a portable way.
 */
#define ___constant_swab16(x) ((__u16)(				\
	(((__u16)(x) & (__u16)0x00ffU) << 8) |			\
	(((__u16)(x) & (__u16)0xff00U) >> 8)))

#define ___constant_swab32(x) ((__u32)(				\
	(((__u32)(x) & (__u32)0x000000ffUL) << 24) |		\
	(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) |		\
	(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) |		\
	(((__u32)(x) & (__u32)0xff000000UL) >> 24)))

#define ___constant_swab64(x) ((__u64)(				\
	(((__u64)(x) & (__u64)0x00000000000000ffULL) << 56) |	\
	(((__u64)(x) & (__u64)0x000000000000ff00ULL) << 40) |	\
	(((__u64)(x) & (__u64)0x0000000000ff0000ULL) << 24) |	\
	(((__u64)(x) & (__u64)0x00000000ff000000ULL) <<  8) |	\
	(((__u64)(x) & (__u64)0x000000ff00000000ULL) >>  8) |	\
	(((__u64)(x) & (__u64)0x0000ff0000000000ULL) >> 24) |	\
	(((__u64)(x) & (__u64)0x00ff000000000000ULL) >> 40) |	\
	(((__u64)(x) & (__u64)0xff00000000000000ULL) >> 56)))

#define ___constant_swahw32(x) ((__u32)(			\
	(((__u32)(x) & (__u32)0x0000ffffUL) << 16) |		\
	(((__u32)(x) & (__u32)0xffff0000UL) >> 16)))

#define ___constant_swahb32(x) ((__u32)(			\
	(((__u32)(x) & (__u32)0x00ff00ffUL) << 8) |		\
	(((__u32)(x) & (__u32)0xff00ff00UL) >> 8)))

/*
 * Implement the following as inlines, but define the interface using
 * macros to allow constant folding when possible:
 * ___swab16, ___swab32, ___swab64, ___swahw32, ___swahb32
 */

static inline __u16 __fswab16(__u16 val)
{
	return ___constant_swab16(val);
}

static inline __u32 __fswab32(__u32 val)
{
	return ___constant_swab32(val);
}

static inline __u64 __fswab64(__u64 val)
{
	return ___constant_swab64(val);
}

static inline __u32 __fswahw32(__u32 val)
{
	return ___constant_swahw32(val);
}

static inline __u32 __fswahb32(__u32 val)
{
	return ___constant_swahb32(val);
}

/**
 * __swab16 - return a byteswapped 16-bit value
 * @x: value to byteswap
 */
#define __swab16(x)				\
	(__builtin_constant_p((__u16)(x)) ?	\
	___constant_swab16(x) :			\
	__fswab16(x))

/**
 * __swab32 - return a byteswapped 32-bit value
 * @x: value to byteswap
 */
#define __swab32(x)				\
	(__builtin_constant_p((__u32)(x)) ?	\
	___constant_swab32(x) :			\
	__fswab32(x))

/**
 * __swab64 - return a byteswapped 64-bit value
 * @x: value to byteswap
 */
#define __swab64(x)				\
	(__builtin_constant_p((__u64)(x)) ?	\
	___constant_swab64(x) :			\
	__fswab64(x))


static inline unsigned long __swab(const unsigned long y)
{
#if __BITS_PER_LONG == 64
	return __swab64(y);
#else /* __BITS_PER_LONG == 32 */
	return __swab32(y);
#endif
}

/**
 * __swahw32 - return a word-swapped 32-bit value
 * @x: value to wordswap
 *
 * __swahw32(0x12340000) is 0x00001234
 */
#define __swahw32(x)				\
	(__builtin_constant_p((__u32)(x)) ?	\
	___constant_swahw32(x) :		\
	__fswahw32(x))

/**
 * __swahb32 - return a high and low byte-swapped 32-bit value
 * @x: value to byteswap
 *
 * __swahb32(0x12345678) is 0x34127856
 */
#define __swahb32(x)				\
	(__builtin_constant_p((__u32)(x)) ?	\
	___constant_swahb32(x) :		\
	__fswahb32(x))

/**
 * __swab16p - return a byteswapped 16-bit value from a pointer
 * @p: pointer to a naturally-aligned 16-bit value
 */
static inline __u16 __swab16p(const __u16 *p)
{
	return __swab16(*p);
}

/**
 * __swab32p - return a byteswapped 32-bit value from a pointer
 * @p: pointer to a naturally-aligned 32-bit value
 */
static inline __u32 __swab32p(const __u32 *p)
{
	return __swab32(*p);
}

/**
 * __swab64p - return a byteswapped 64-bit value from a pointer
 * @p: pointer to a naturally-aligned 64-bit value
 */
static inline __u64 __swab64p(const __u64 *p)
{
	return __swab64(*p);
}

/**
 * __swahw32p - return a wordswapped 32-bit value from a pointer
 * @p: pointer to a naturally-aligned 32-bit value
 *
 * See __swahw32() for details of wordswapping.
 */
static inline __u32 __swahw32p(const __u32 *p)
{
	return __swahw32(*p);
}

/**
 * __swahb32p - return a high and low byteswapped 32-bit value from a pointer
 * @p: pointer to a naturally-aligned 32-bit value
 *
 * See __swahb32() for details of high/low byteswapping.
 */
static inline __u32 __swahb32p(const __u32 *p)
{
	return __swahb32(*p);
}

/**
 * __swab16s - byteswap a 16-bit value in-place
 * @p: pointer to a naturally-aligned 16-bit value
 */
static inline void __swab16s(__u16 *p)
{
	*p = __swab16p(p);
}
/**
 * __swab32s - byteswap a 32-bit value in-place
 * @p: pointer to a naturally-aligned 32-bit value
 */
static inline void __swab32s(__u32 *p)
{
	*p = __swab32p(p);
}

/**
 * __swab64s - byteswap a 64-bit value in-place
 * @p: pointer to a naturally-aligned 64-bit value
 */
static inline void __swab64s(__u64 *p)
{
	*p = __swab64p(p);
}

/**
 * __swahw32s - wordswap a 32-bit value in-place
 * @p: pointer to a naturally-aligned 32-bit value
 *
 * See __swahw32() for details of wordswapping
 */
static inline void __swahw32s(__u32 *p)
{
	*p = __swahw32p(p);
}

/**
 * __swahb32s - high and low byteswap a 32-bit value in-place
 * @p: pointer to a naturally-aligned 32-bit value
 *
 * See __swahb32() for details of high and low byte swapping
 */
static inline void __swahb32s(__u32 *p)
{
	*p = __swahb32p(p);
}


#endif /* _UAPI_LINUX_SWAB_H */
