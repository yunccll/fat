#include "fat_super.h"
#include "base.h"
#include "mock/msdos_fs.h"
#include "mock/access_ok.h"
//#include "mock/ffs.h"

void fat_sb_free(struct fat_sb * fsb)
{
    free(fsb);
}

static void fat_sb_print(struct fat_sb * fsb)
{
    //TODO:
}

static int fat_sb_read(struct fat_sb  *fsb, const struct fat_boot_sector * fbs)
{
	fsb->sector_size = get_unaligned_le16(fbs->sector_size);

	fsb->fat_bits = 12;
	fsb->fsinfo_sector = 1;

	fsb->fat_sector_start = le16_to_cpu(fbs->reserved); 
	fsb->fat_length = le16_to_cpu(fbs->fat_length);
	fsb->fats = fbs->fats;

	fsb->root_dir_sector_start = fsb->fat_sector_start + fbs->fats * fsb->fat_length;
	fsb->root_dir_entries = get_unaligned_le16(&fbs->dir_entries);

	fsb->data_sector_start = fsb->root_dir_sector_start + fsb->root_dir_entries * fsb->sector_size / sizeof(struct msdos_dir_entry);

	fsb->max_sectors = get_unaligned_le16(&fbs->sectors);
	if(fsb->max_sectors == 0)
		fsb->max_sectors = le32_to_cpu(fbs->total_sect);


	fsb->sec_per_clus = fbs->sec_per_clus;
	fsb->root_cluster = 0;
	//TODO: fsb->max_cluster =  ??????

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

    fsb = (struct fat_sb*)calloc(sizeof(struct fat_sb), 1);
    if(fsb && !fat_sb_read(fsb, (const struct fat_boot_sector*)data)){
        fat_sb_print(fsb);
        return fsb;
    }
    fat_sb_free(fsb);
    return NULL;
}
