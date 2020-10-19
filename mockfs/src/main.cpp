#include <gtest/gtest.h>
#include "lfs_uti.h"

//TODO: fat as a normal file (page-cached read/write, sync)

struct intf_pages {
};
struct intf_pages * read_pages(void * dev, int start, int len){
    return NULL;
}
void free_pages(struct intf_pages * pages){
}

struct lfs_fat {
   char * pages;
};


void lfs_fat_free(struct lfs_fat * fat){
    free(fat);
}
struct lfs_fat * lfs_fat_create(){
    return (struct lfs_fat*) calloc(1, sizeof(struct lfs_fat));
}
int lfs_fat_fill(struct lfs_fat * fat, struct intf_pages * pages){
    fat->pages = pages;
    return 0;
}


sector_t lfs_fat_get_next_cluster(struct lfs_fat * fat, sector_t cur){
    if(is_even(cur))
        int offset = cluster_offset_even(cur);
        return  byte_array_get_uint16(fat->pages, offset) & 0xfff;
    }
    else {
       int offset = cluster_offset_odd(cur);
       return byte_array_get_uint16(fat->pages, offset) >> 4;
    }
}
void lfs_fat_set_next_cluster(struct lfs_fat * fat, sector_t cur, sector_t next){
}
int lfs_fat_is_last_cluster(struct lfs_fat * fat, sector_t cur){
    return TRUE;
}
int lfs_fat_is_bad_cluster(struct lfs_fat * fat, sector_t cur){
    return FALSE;
}

int lfs_fat_get_first_idle_cluster(struct lfs_fat * fat){
    return 0;
}


int lfs_fat_sync(struct lfs_fat * fat, struct intf_pages * pages){
    return 0;
}




TEST(lfs_fat_test, mount){
    const char * dev = "/dev/floppy";
    struct intf_pages * pages = read_pages((void *)dev, 2, 9);

    //1. load pages to lfs_fat
    struct lfs_fat * fat = lfs_fat_create();

    lfs_fat_fill(fat, pages);


    //2. get/set next cluster
    const int cluster_no = 100;
    const int next_cluster_no = lfs_fat_get_next_cluster(fat, cluster_no);
    ASSERT_EQ(next_cluster_no, 101);

    lfs_fat_set_next_cluster(fat, cluster_no, 102);
    ASSERT_EQ(102, lfs_fat_get_next_cluster(fat, cluster_no));


    //3. find first idle  cluster No
    const int idle_cluster_no = lfs_fat_get_first_idle_cluster(fat);
    ASSERT_EQ(idle_cluster_no, 0);

    //4. sync
    lfs_fat_sync(fat, pages);

    lfs_fat_free(fat);

    free_pages(pages);
}


