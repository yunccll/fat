#include <gtest/gtest.h>
#include "mock.h"

struct lfs_sbp {

};
void lfs_sbp_free(struct lfs_sbp * sbp){
    free(sbp);
}
struct lfs_sbp *  lfs_sbp_create(){
    return (struct lfs_sbp*)calloc(1, sizeof(struct lfs_sbp));
}
void lfs_sbp_init(struct lfs_sbp* sbp, char * data, int size){
    //TODO: sbp->
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


    return 0;
}



