
#ifndef  UAPI_LINUX_MSDOS_FS_H
#define  UAPI_LINUX_MSDOS_FS_H

#include "mock/linux/types.h"

#define FAT_START_ENT   2

#define MSDOS_NAME  11

struct fat_boot_sector {
    __u8    ignored[3]; /* Boot strap short or near jump */
    __u8    system_id[8];   /* Name - can be used to special case
                   partition manager volumes */
    __u8    sector_size[2]; /* bytes per logical sector */
    __u8    sec_per_clus;   /* sectors/cluster */
    __le16  reserved;   /* reserved sectors */
    __u8    fats;       /* number of FATs */
    __u8    dir_entries[2]; /* root directory entries */
    __u8    sectors[2]; /* number of sectors */
    __u8    media;      /* media code */
    __le16  fat_length; /* sectors/FAT */
    __le16  secs_track; /* sectors per track */
    __le16  heads;      /* number of heads */
    __le32  hidden;     /* hidden sectors (unused) */
    __le32  total_sect; /* number of sectors (if sectors == 0) */

	 union {
        struct {
            /*  Extended BPB Fields for FAT16 */
            __u8    drive_number;   /* Physical drive number */
            __u8    state;      /* undocumented, but used
                           for mount state. */
            __u8    signature;  /* extended boot signature */
            __u8    vol_id[4];  /* volume ID */
            __u8    vol_label[MSDOS_NAME];  /* volume label */
            __u8    fs_type[8];     /* file system type */
            /* other fields are not added here */
        } fat16;
		struct {
            /* only used by FAT32 */
            __le32  length;     /* sectors/FAT */
            __le16  flags;      /* bit 8: fat mirroring,
                           low 4: active fat */
            __u8    version[2]; /* major, minor filesystem
                           version */
            __le32  root_cluster;   /* first cluster in
                           root directory */
            __le16  info_sector;    /* filesystem info sector */
            __le16  backup_boot;    /* backup boot sector */
            __le16  reserved2[6];   /* Unused */
            /* Extended BPB Fields for FAT32 */
            __u8    drive_number;   /* Physical drive number */
            __u8    state;          /* undocumented, but used
                           for mount state. */
            __u8    signature;  /* extended boot signature */
            __u8    vol_id[4];  /* volume ID */
            __u8    vol_label[MSDOS_NAME];  /* volume label */
            __u8    fs_type[8];     /* file system type */
            /* other fields are not added here */
        } fat32;
    };

};

struct msdos_dir_entry {
    __u8    name[MSDOS_NAME];/* name and extension */
    __u8    attr;       /* attribute bits */
    __u8    lcase;      /* Case for base and extension */
    __u8    ctime_cs;   /* Creation time, centiseconds (0-199) */
    __le16  ctime;      /* Creation time */
    __le16  cdate;      /* Creation date */
    __le16  adate;      /* Last access date */
    __le16  starthi;    /* High 16 bits of cluster in FAT32 */
    __le16  time,date,start;/* time, date and first cluster */
    __le32  size;       /* file size (in bytes) */
};



#endif   /* UAPI_LINUX_MSDOS_FS_H */
