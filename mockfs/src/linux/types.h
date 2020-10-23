/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LINUX_TYPES_H
#define LINUX_TYPES_H

typedef unsigned char       u8;
typedef char                s8;
typedef unsigned short      u16;
typedef short               s16;
typedef unsigned  int       u32;
typedef int                 s32;
typedef unsigned long long  u64;
typedef long long           s64;



typedef u32 __kernel_dev_t;

//typedef __kernel_fd_set		fd_set;
//typedef __kernel_dev_t		dev_t;
//typedef __kernel_ino_t		ino_t;
//typedef __kernel_mode_t		mode_t;
typedef unsigned short		umode_t;
//typedef __kernel_off_t		off_t;
//typedef __kernel_pid_t		pid_t;
//typedef __kernel_daddr_t	daddr_t;
//typedef __kernel_key_t		key_t;
//typedef __kernel_suseconds_t	suseconds_t;
//typedef __kernel_timer_t	timer_t;
//typedef __kernel_clockid_t	clockid_t;
//typedef __kernel_mqd_t		mqd_t;

//typedef _Bool			bool;
//
//typedef __kernel_uid32_t	uid_t;
//typedef __kernel_gid32_t	gid_t;
//typedef __kernel_uid16_t        uid16_t;
//typedef __kernel_gid16_t        gid16_t;

typedef unsigned long		uintptr_t;

/* bsd */
typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned int		u_int;
typedef unsigned long		u_long;

/* sysv */
typedef unsigned char		unchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;

typedef u8			u_int8_t;
typedef u16			u_int16_t;
typedef u32			u_int32_t;

/**
 * The type used for indexing onto a disc or disc partition.
 *
 * Linux always considers sectors to be 512 bytes long independently
 * of the devices real block size.
 *
 * blkcnt_t is the type of the inode's block count.
 */
typedef u64 sector_t;

/*
 * The type of an index into the pagecache.
 */
#define pgoff_t unsigned long

/*
 * A dma_addr_t can hold any valid DMA address, i.e., any address returned
 * by the DMA API.
 *
 * If the DMA API only uses 32-bit addresses, dma_addr_t need only be 32
 * bits wide.  Bus addresses, e.g., PCI BARs, may be wider than 32 bits,
 * but drivers do memory-mapped I/O to ioremapped kernel virtual addresses,
 * so they don't care about the size of the actual bus addresses.
 */
typedef u64 dma_addr_t;

typedef unsigned int __bitwise gfp_t;
typedef unsigned int __bitwise slab_flags_t;
typedef unsigned int __bitwise fmode_t;

typedef u64 phys_addr_t;
typedef phys_addr_t resource_size_t;

/*
 * This type is the placeholder for a hardware interrupt number. It has to be
 * big enough to enclose whatever representation is used by a given platform.
 */
typedef unsigned long irq_hw_number_t;

typedef struct {
	int counter;
} atomic_t;

typedef struct {
	s64 counter;
} atomic64_t;

struct list_head {
	struct list_head *next, *prev;
};

struct hlist_head {
	struct hlist_node *first;
};

struct hlist_node {
	struct hlist_node *next, **pprev;
};

//struct ustat {
//	__kernel_daddr_t	f_tfree;
//	__kernel_ino_t		f_tinode;
//	char			f_fname[6];
//	char			f_fpack[6];
//};

/**
 * struct callback_head - callback structure for use with RCU and task_work
 * @next: next update requests in a list
 * @func: actual update function to call after the grace period.
 *
 * The struct is aligned to size of pointer. On most architectures it happens
 * naturally due ABI requirements, but some architectures (like CRIS) have
 * weird ABI and we need to ask it explicitly.
 *
 * The alignment is required to guarantee that bit 0 of @next will be
 * clear under normal conditions -- as long as we use call_rcu() or
 * call_srcu() to queue the callback.
 *
 * This guarantee is important for few reasons:
 *  - future call_rcu_lazy() will make use of lower bits in the pointer;
 *  - the structure shares storage space in struct page with @compound_head,
 *    which encode PageTail() in bit 0. The guarantee is needed to avoid
 *    false-positive PageTail().
 */
//struct callback_head {
//	struct callback_head *next;
//	void (*func)(struct callback_head *head);
//} __attribute__((aligned(sizeof(void *))));
//#define rcu_head callback_head
//
//typedef void (*rcu_callback_t)(struct rcu_head *head);
//typedef void (*call_rcu_func_t)(struct rcu_head *head, rcu_callback_t func);

#endif /* LINUX_TYPES_H */
