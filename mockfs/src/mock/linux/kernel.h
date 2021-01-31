
#ifndef  LINUX_KERNEL_H
#define  LINUX_KERNEL_H

/* *
    BUILD_BUG_ON_MSG(!__same_type(*(ptr), ((type *)0)->member) &&   \
        !__same_type(*(ptr), void),            \
        "pointer type mismatch in container_of()");    \
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:  the type of the container struct this is embedded in.
 * @member:   the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({              \
    void *__mptr = (void *)(ptr);                   \
    ((type *)(__mptr - offsetof(type, member)));    \
    })

#endif   /* LINUX_KERNEL_H */
