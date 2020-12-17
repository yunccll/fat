
#ifndef  BASE_H
#define  BASE_H


#undef pr_fmt
#define pr_fmt(fmt) "%s:%s:%d: " fmt, KBUILD_MODNAME, __func__, __LINE__


#include <linux/list.h>
#include <linux/slab.h>
#include <linux/string.h>

#endif   /* BASE_H */
