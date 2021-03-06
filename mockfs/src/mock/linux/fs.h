#ifndef  LINUX_FS_H
#define  LINUX_FS_H

#include "mock/linux/uidgid.h"


static inline kuid_t current_uid()
{
    return KUIDT_INIT(100);
}
static inline kgid_t current_gid()
{
    return KGIDT_INIT(100);
}



#define MAX_NON_LFS ((1UL<<31) - 1)

#define __force     
/*  file is open for reading */
#define FMODE_READ      ((__force fmode_t)0x1)
/*  file is open for writing */
#define FMODE_WRITE     ((__force fmode_t)0x2)
/*  file is seekable */
#define FMODE_LSEEK     ((__force fmode_t)0x4)
/*  file can be accessed using pread */
#define FMODE_PREAD     ((__force fmode_t)0x8)
/*  file can be accessed using pwrite */
#define FMODE_PWRITE        ((__force fmode_t)0x10)
/*  File is opened for execution with sys_execve / sys_uselib */
#define FMODE_EXEC      ((__force fmode_t)0x20)
/*  File is opened with O_NDELAY (only set for block devices) */
#define FMODE_NDELAY        ((__force fmode_t)0x40)
/*  File is opened with O_EXCL (only set for block devices) */
#define FMODE_EXCL      ((__force fmode_t)0x80)
/*  File is opened using open(.., 3, ..) and is writeable only for ioctls
 *     (specialy hack for floppy.c) */
#define FMODE_WRITE_IOCTL   ((__force fmode_t)0x100)
/*  32bit hashes as llseek() offset (for directories) */
#define FMODE_32BITHASH         ((__force fmode_t)0x200)
/*  64bit hashes as llseek() offset (for directories) */
#define FMODE_64BITHASH         ((__force fmode_t)0x400)


/*
 *  * sb->s_flags.  Note that these mirror the equivalent MS_* flags where
 *   * represented in both.
 *    */
#define SB_RDONLY    1  /*  Mount read-only */
#define SB_NOSUID    2  /*  Ignore suid and sgid bits */
#define SB_NODEV     4  /*  Disallow access to device special files */
#define SB_NOEXEC    8  /*  Disallow program execution */
#define SB_SYNCHRONOUS  16  /*  Writes are synced at once */
#define SB_MANDLOCK 64  /*  Allow mandatory locks on an FS */
#define SB_DIRSYNC  128 /*  Directory modifications are synchronous */
#define SB_NOATIME  1024    /*  Do not update access times. */
#define SB_NODIRATIME   2048    /*  Do not update directory access times */
#define SB_SILENT   32768
#define SB_POSIXACL (1<<16) /*  VFS does not apply the umask */
#define SB_KERNMOUNT    (1<<22) /*  this is a kern_mount call */
#define SB_I_VERSION    (1<<23) /*  Update inode I_version field */
#define SB_LAZYTIME (1<<25) /*  Update the on-disk [acm]times lazily */

/*  These sb flags are internal to the kernel */
#define SB_SUBMOUNT     (1<<26)
#define SB_FORCE        (1<<27)
#define SB_NOSEC    (1<<28)
#define SB_BORN     (1<<29)
#define SB_ACTIVE   (1<<30)
#define SB_NOUSER   (1<<31)


#endif   /* LINUX_FS_H */
