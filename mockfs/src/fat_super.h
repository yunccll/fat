
#ifndef  FAT_SUPER_H
#define  FAT_SUPER_H

#include "mock/linux/types.h"
#include "super.h"


struct fat_sb {
	u16 sector_size;	/* sector size in disk */

	u8 fat_bits; 	/* FAT bits(12, 16, 32) */

	u32 fsinfo_sector;	/* sector number of FAT32 fsinfo */	

	u16 fat_sector_start;
	u32 fat_length;
	u8 fats; 		/* number of fats */

	u32 root_dir_sector_start;	/* root directory sector start*/
	u16 root_dir_entries;

	u32 data_sector_start;	/* first data sector start */
	u32 max_sectors;	/* max sectors in disk */


    u16 sec_per_clus;	/* sectors per cluster */
	u32 root_cluster; 	/* first cluster of the root directory */
	u32 max_cluster;	/* maximum cluster number */
	u32 cluster_size; 	
	u16 cluster_bits;	/* log2(cluster_size) */

	s32 dir_per_block;        /* dir entries per block */
    s32 dir_per_block_bits;       /* log2(dir_per_block) */
	
};

static inline void fat_sb_save(struct super_block * sb, struct fat_sb * fsb)
{
    if(fsb)
        sb->s_fs_info = fsb;
}
static inline struct fat_sb * fat_sb(struct super_block * sb){
    return (struct fat_sb*)sb->s_fs_info;
}


void fat_sb_free(struct fat_sb * fsb);
struct fat_sb * fat_sb_create(u8 * data, unsigned long size);


#endif   /* FAT_SUPER_H */
