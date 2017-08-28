
#ifndef  BOOT_H
#define  BOOT_H


#include "common.h"
#include "device.h"

#define BOOTCODE_SIZE       448


struct msdos_volume_info {
    __u8        drive_number;       //  BIOS drive number
    __u8        RESERVED;           //  Unused
    __u8        ext_boot_sign;      //  0x29 if fields below exist (DOS 3.3+)
    __u8        volume_id[4];       //  Volume ID number
    __u8        volume_label[11];   //  Volume label
    __u8        fs_type[8];         //  Typically FAT12 or FAT16
} __attribute__ ((packed));

struct fat_boot {
    __u8          jmp[3];           // 0xeb 0x3c 0x90
    __u8          sys_id[8];        // system id 
    __u8          sector_size[2];   //  bytes per logical sector
    __u8          cluster_size;     //  sectors per cluster
    __u16         reserved;         //  reserved sectors
    __u8          fats;             //  number of FATs
	__u8          dir_entries[2];   //  root directory entries
    __u8          sectors[2];       //  number of sectors
    __u8          media;            //  media code (unused)
    __u16         fat_length;       //  sectors per FAT
    __u16         secs_track;       //  sectors per track
    __u16         heads;            //  number of heads
    __u32         hidden;           //  hidden sectors (unused)
    __u32         total_sect;       //  number of sectors (if sectors == 0)
    struct msdos_volume_info vi;    //  msdos volumn info
    __u8   boot_code[BOOTCODE_SIZE];//  boot code 
    __u16        boot_sign;         //  boot sign 0x AA 55 low -> high
}__attribute__((__packed__));
typedef struct fat_boot fat_boot_t;


void fat_boot_print(fat_boot_t * pb);


void fat_boot_init(fat_boot_t * pb);

void fat_boot_format12(fat_boot_t * pb);


int fat_boot_read(fat_boot_t * pb, const char * file, fat_offset_t offset);
int fat_boot_write(fat_boot_t * pb, const char * file, fat_offset_t offset);
int fat_boot_write_to_dev(fat_boot_t * pb, fat_dev_t * pdev, fat_offset_t offset);

#define  DECLARE_FAT_BOOT(name) \
    fat_boot_t name;    \
    fat_boot_init(&name)
        


// macro for fat1 and fat2 length
#define BOOT_SECTOR_SIZE           1

#define fat1_start_sector               BOOT_SECTOR_SIZE

#define fat_end_sector(start,fat_len)   ((start) + (fat_len) - 1)
#define fat1_end_sector(fat_len)        fat_end_sector(fat1_start_sector, fat_len)

#define fat2_start_sector(fat_len)      (fat1_end_sector(fat_len) + 1)
#define fat2_end_sector(fat_len)        fat_end_sector(fat2_start_sector(fat_len), fat_len)

// calculation from boot value
#define FAT_DIR_ENTRY_SIZE              32
#define fat_root_dir_sectno(pb)           ( (*((short*)((pb)->dir_entries))) * FAT_DIR_ENTRY_SIZE/512)

#define fat_root_dir_start_sectno(pb)   (BOOT_SECTOR_SIZE+(pb)->fat_length*(pb)->fats)
#define fat_root_dir_end_sectno(pb)     (fat_root_dir_start_sectno(pb) + fat_root_dir_sectno(pb)-1)


#endif   /* BOOT_H */
