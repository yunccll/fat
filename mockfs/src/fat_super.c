#include "fat_super.h"

#include "mock/uapi/linux/msdos_fs.h"
#include "mock/unaligned/access_ok.h"
#include "mock/asm-generic/bitops/ffs.h"
#include "mock/linux/printk.h"
#include "mock/linux/slab.h"

void fat_sb_free(struct fat_sb * fsb)
{
    kfree(fsb);
}

static void fat_sb_print(struct fat_sb * fsb)
{
	pr_debug("DEBUG: sector_size in disk - [%d]\n", fsb->sector_size);

	pr_debug("DEBUG: fat bits (12, 16 32) - [%d]\n",fsb->fat_bits);

	pr_debug("DEBUG: sector number of fsinfo - [%d]\n", fsb->fsinfo_sector);

	pr_debug("DEBUG: fat sector start - [%d]\n", fsb->fat_sector_start);
	pr_debug("DEBUG: fat length - [%d]\n", fsb->fat_length);
	pr_debug("DEBUG: number of fats - [%d]\n", fsb->fats);

	pr_debug("DEBUG: root dir sector start - [%d]\n", fsb->root_dir_sector_start);
	pr_debug("DEBUG: number of entry in root_directory - [%d]\n", fsb->root_dir_entries);

	pr_debug("DEBUG: first data sector - [%d]\n", fsb->data_sector_start);
	pr_debug("DEBUG: max sectors in disk - [%d]\n", fsb->max_sectors);

	pr_debug("DEBUG: number of sectors per cluster - [%d]\n", fsb->sec_per_clus);
	pr_debug("DEBUG: first cluster of root cluster - [%d]\n", fsb->root_cluster);
	pr_debug("DEBUG: maximum cluster number - [%d]\n", fsb->max_cluster);

	pr_debug("DEBUG: cluster_size - [%d]\n", fsb->cluster_size); 	
	pr_debug("DEBUG: log2(cluster_size) - [%d]\n", fsb->cluster_bits);
}

static u32 calc_max_sectors(const struct fat_boot_sector * fbs)
{
	u32 max_sectors = get_unaligned_le16(&fbs->sectors);
	if(max_sectors == 0)
		max_sectors = le32_to_cpu(fbs->total_sect);
	return max_sectors;
}
static u32 calc_max_cluster(struct fat_sb * fsb) 
{
	u32 total_cluster = (fsb->max_sectors - fsb->data_sector_start)	/ fsb->sec_per_clus;
	u32 fat_cluster = fsb->fat_length * fsb->sector_size * 8 / fsb->fat_bits;
	pr_debug("----total_cluster:%d, fat_cluster:%d\n", total_cluster, fat_cluster);
	return min(total_cluster, fat_cluster - FAT_START_ENT) + FAT_START_ENT;
}
static int fat_sb_read(struct fat_sb  *fsb, const struct fat_boot_sector * fbs)
{
	fsb->sector_size = get_unaligned_le16(fbs->sector_size);

	fsb->fat_bits = 12;
	fsb->fsinfo_sector = 0;

	fsb->fat_sector_start = le16_to_cpu(fbs->reserved); 
	fsb->fat_length = le16_to_cpu(fbs->fat_length);
	fsb->fats = fbs->fats;

	fsb->root_dir_sector_start = fsb->fat_sector_start + fbs->fats * fsb->fat_length;
	fsb->root_dir_entries = get_unaligned_le16(&fbs->dir_entries);

	fsb->data_sector_start = fsb->root_dir_sector_start + fsb->root_dir_entries *  sizeof(struct msdos_dir_entry)/fsb->sector_size;

	fsb->max_sectors = calc_max_sectors(fbs);

	fsb->sec_per_clus = fbs->sec_per_clus;
	fsb->root_cluster = 0;

	fsb->max_cluster = calc_max_cluster(fsb);

	fsb->cluster_size = (fsb->sec_per_clus) * (fsb->sector_size);
	fsb->cluster_bits = ffs(fsb->cluster_size) - 1;
    return 0;
}
struct fat_sb * fat_sb_create(u8 * data, unsigned long size)
{
    struct fat_sb * fsb;
    if(!data){
        return NULL;
    }
    pr_debug("MRB info %x,%x,%x\n", data[0], data[1], data[2]);

    fsb = (struct fat_sb*)kzalloc(sizeof(struct fat_sb), GFP_KERNEL);
    if(fsb && !fat_sb_read(fsb, (const struct fat_boot_sector*)data)){
        fat_sb_print(fsb);
        return fsb;
    }
    fat_sb_free(fsb);
    return NULL;
}
