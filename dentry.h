#ifndef  DENTRY_H
#define  DENTRY_H

#include "common.h"


struct dentry
{
    char  name[8], ext[3];    /*  name and extension */
    __u8        attr;         /*  attribute bits */
    __u8  lcase;          /*  Case for base and extension */
    __u8  ctime_ms;       /*  Creation time, milliseconds */
    __u16 ctime;          /*  Creation time */
    __u16 cdate;          /*  Creation date */
    __u16 adate;          /*  Last access date */
    __u16 starthi;        /*  high 16 bits of first cl. (FAT32) */
    union {
        __u16 val;
        struct {
        __u16 second:5;
        __u16 minute:6;
        __u16 hour:5;
        } detail;
    } time;
    union {
        __u16 val;
        struct {
        __u16 day:5;
        __u16 month:4;
        __u16 year:7;
        } detail;
    } date;
    __u16 start;  /*  time, date and first cluster */
    __u32 size;           /*  file size (in bytes) */
} __attribute__ ((packed));

typedef struct dentry fat_dentry_t;

#endif   /* DENTRY_H */
