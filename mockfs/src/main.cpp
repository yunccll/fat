#include <gtest/gtest.h>
//TODO: fat as a normal file (page-cached read/write, sync)


#define TRUE  1
#define FALSE 0


struct intf_pages {
};
struct intf_pages * read_pages(void * dev, int start, int len){
    return NULL;
}
void free_pages(struct intf_pages * pages){
}

struct lfs_fat {
};


void lfs_fat_free(struct lfs_fat * fat){
    free(fat);
}
struct lfs_fat * lfs_fat_create(){
    return (struct lfs_fat*) malloc(sizeof(struct lfs_fat));
}
int lfs_fat_fill(struct lfs_fat * fat, struct intf_pages * pages){
    return 0;
}
int lfs_fat_get_next_cluster(struct lfs_fat * fat, int cluster_no){
    return 0;
}
void lfs_fat_set_next_cluster(struct lfs_fat * fat, int cluster_no, int next_cluster_no){
}
int lfs_fat_is_last_cluster(struct lfs_fat * fat, int cluster_no){
    return TRUE;
}
int lfs_fat_is_bad_cluster(struct lfs_fat * fat, int cluster_no){
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


