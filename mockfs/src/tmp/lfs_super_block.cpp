#include <gtest/gtest.h>
#include "mock.h"

#include <linux/msdos_fs.h>

struct lfs_sbp {
    uint16_t sector_size;
    uint8_t  sec_per_clus;
    uint16_t reserved;
    uint8_t  fats;
    uint16_t dir_entries;
    uint16_t sectors;
    uint16_t fat_length;
    uint32_t total_sect;

};
void lfs_sbp_free(struct lfs_sbp * sbp){
    free(sbp);
}
struct lfs_sbp *  lfs_sbp_create(){
    return (struct lfs_sbp*)calloc(1, sizeof(struct lfs_sbp));
}
void lfs_sbp_init(struct lfs_sbp* sbp, char * data, int size){
    struct fat_boot_sector * b = (struct fat_boot_sector *)data;
    sbp->sector_size = get_unaligned_le16(&b->sector_size);
    sbp->sec_per_clus = b->sec_per_clus;
    sbp->reserved = le16_to_cpu(b->reserved);
    sbp->fats = b->fats;
    sbp->dir_entries = get_unaligned_le16(&b->dir_entries);
    sbp->sectors = get_unaligned_le16(&b->sectors);
    sbp->fat_length = le16_to_cpu(b->fat_length);
    sbp->total_sect = le32_to_cpu(b->total_sect);
    //TODO: check  sector_size , sec_per_clus
//    sbi->dir_per_block = sb->s_blocksize / sizeof(struct msdos_dir_entry);
//    sbi->dir_per_block_bits = ffs(sbi->dir_per_block) - 1;
//    sbi->dir_start = sbi->fat_start + sbi->fats * sbi->fat_length;
//    sbi->dir_entries = bpb.fat_dir_entries;
//    rootdir_sectors = sbi->dir_entries
//        * sizeof(struct msdos_dir_entry) / sb->s_blocksize;
//    sbi->data_start = sbi->dir_start + rootdir_sectors;
//    total_sectors = bpb.fat_sectors;
//    if (total_sectors == 0)
//        total_sectors = bpb.fat_total_sect;
//
//    total_clusters = (total_sectors - sbi->data_start) / sbi->sec_per_clus;
//      sbi->fat_bits = (total_clusters > MAX_FAT12) ? 16 : 12;

//  fat_clusters = calc_fat_clusters(sb);
//    total_clusters = min(total_clusters, fat_clusters - FAT_START_ENT);
//    if (total_clusters > max_fat(sb)) {
//        if (!silent)
//            fat_msg(sb, KERN_ERR, "count of clusters too big (%u)",
//                   total_clusters);
//        goto out_invalid;
//    }
//
//    sbi->max_cluster = total_clusters + FAT_START_ENT;
//    /* check the free_clusters, it's not necessarily correct */
//    if (sbi->free_clusters != -1 && sbi->free_clusters > total_clusters)
//        sbi->free_clusters = -1;
//    /* check the prev_free, it's not necessarily correct */
//    sbi->prev_free %= sbi->max_cluster;
//    if (sbi->prev_free < FAT_START_ENT)
//        sbi->prev_free = FAT_START_ENT;
}

int lfs_fill_super(struct super_block * sb, void * data, int silent){

    //1. read boot sector init for sb
    sb_min_blocksize(sb, 512);
    struct buffer_head * bh = sb_bread(sb, 0);
    if(!bh) return -ENOMEM;

    struct lfs_sbp * sbp = lfs_sbp_create();
    if(!sbp){
        brelse(bh);
        return -ENOMEM;
    }

    //parse the boot sector to sb->s_fs_info
    lfs_sbp_init(sbp, bh->b_data, bh->b_size);
    sb->s_fs_info = sbp;

    brelse(bh);

    //2. read fat sectors

    //3. root_dentry create
    //sb->s_root = lfs_root_dentry_create(); //TODO:

    return 0;
}
void lfs_kill_super(struct super_block * sb){
    kill_block_super(sb); //for mount_bdev

    /*  
    lfs_root_dentry_destroy(sb->s_root); //TODO: 
    sb->s_root = NULL;
    */
    
    lfs_sbp_free((struct lfs_sbp*)sb->s_fs_info); //for lfs_super_block_create
    sb->s_fs_info = NULL;
}



