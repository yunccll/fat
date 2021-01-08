#include <gtest/gtest.h>
#include "util.h"
#include <linux/msdos_fs.h>

//TODO: fat as a normal file (page-cached read/write, sync)

struct intf_pages {
    uint8_t * pages;
    uint64_t used;
};
struct intf_pages * read_pages(void * dev, int start, int len){
    struct intf_pages * pages = (struct intf_pages*)calloc(1, sizeof(struct intf_pages));
    pages->pages = (uint8_t *)calloc(9, 512*sizeof(uint8_t));
    //init data
    pages->pages[0] = 0x12;
    pages->pages[1] = 0x34;
    pages->pages[2] = 0x56;
    pages->pages[3] = 0x78;
    pages->pages[4] = 0x9A;
    pages->pages[5] = 0xBC;
    pages->pages[6] = 0xDE;
    pages->pages[7] = 0xF0;
    pages->pages[8] = 0x12;
    pages->pages[9] = 0x00;
    pages->pages[10] = 0x00;
    pages->pages[11] = 0x00;
    pages->used = 12;

    return pages;
}
void free_pages(struct intf_pages * pages){
    if(pages){
        if(pages->pages)
            free(pages->pages);
        free(pages);
    }
}

struct lfs_fat {
   uint8_t * data;
   uint64_t  max_cluster_count;
};



void lfs_fat_free(struct lfs_fat * fat){
    free(fat);
}
struct lfs_fat * lfs_fat_create(){
    return (struct lfs_fat*) calloc(1, sizeof(struct lfs_fat));
}
int lfs_fat_fill(struct lfs_fat * fat, struct intf_pages * pages){
    //TODO: CHECK ????
    fat->data = pages->pages;
    fat->max_cluster_count = (pages->used << 1) / 3;
    return 0;
}


sector_t lfs_fat_get_next_cluster(struct lfs_fat * fat, sector_t cur){
    if(is_even(cur)){ //0
        uint64_t offset = offset_of_cluster_even(cur);
        return byte_array_get_uint16_even(fat->data, offset);
    }
    else { //1
       uint64_t offset = offset_of_cluster_odd(cur);
       return byte_array_get_uint16_odd(fat->data, offset);
    }
}
void lfs_fat_set_next_cluster(struct lfs_fat * fat, sector_t cur, sector_t next){
    if(is_even(cur)){ //0
        uint64_t offset = offset_of_cluster_even(cur);
        byte_array_set_uint16_even(fat->data, offset, next);
    }
    else{ //1
        uint64_t offset = offset_of_cluster_odd(cur);
        byte_array_set_uint16_odd(fat->data, offset, next);
    }
}
int lfs_fat_is_last_cluster(struct lfs_fat * fat, sector_t cur){
    if(cur >= FAT_START_ENT && cur < fat->max_cluster_count){
        return lfs_fat_get_next_cluster(fat, cur) == EOF_FAT12;
    }
    return -1;
}
int lfs_fat_is_bad_cluster(struct lfs_fat * fat, sector_t cur){
    if(cur >= FAT_START_ENT && cur < fat->max_cluster_count){
        return lfs_fat_get_next_cluster(fat, cur) == BAD_FAT12;
    }
    return -1;
}

int lfs_fat_get_first_idle_cluster(struct lfs_fat * fat, sector_t * idle_cluster_no){
    //0, 1 -> no-used
    uint64_t i = FAT_START_ENT;
    for(; i < fat->max_cluster_count; ++i){
        if(FAT_ENT_FREE == lfs_fat_get_next_cluster(fat, i)){
            *idle_cluster_no = i;
            return  0;
        }
    }
    return i >= FAT_START_ENT && i < fat->max_cluster_count ? 0 : -1;
}

int lfs_fat_sync(struct lfs_fat * fat, struct intf_pages * pages){
    //TODO: CHECK ????
    return 0;
}




TEST(lfs_fat_test, mount){
    const char * dev = "/dev/floppy";
    struct intf_pages * pages = read_pages((void *)dev, 2, 9);

    //1. load pages to lfs_fat
    struct lfs_fat * fat = lfs_fat_create();

    lfs_fat_fill(fat, pages);


    //2. get/set next cluster
    const int cluster_no = 3;
    const int next_cluster_no = lfs_fat_get_next_cluster(fat, cluster_no);
    ASSERT_EQ(0xbc9, next_cluster_no);

    lfs_fat_set_next_cluster(fat, cluster_no, 0x064);
    ASSERT_EQ((sector_t)0x064, lfs_fat_get_next_cluster(fat, cluster_no));


    //3. find first idle  cluster No
    sector_t idle_cluster_no = 0;
    ASSERT_EQ(0, lfs_fat_get_first_idle_cluster(fat, &idle_cluster_no));
    ASSERT_EQ((sector_t)6, idle_cluster_no);

    //4. sync
    lfs_fat_sync(fat, pages);

    lfs_fat_free(fat);

    free_pages(pages);
}
