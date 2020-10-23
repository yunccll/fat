#include <gtest/gtest.h>
#include "lfs_super_block.h"

TEST(lfs_fill_super_test, mount){
    struct super_block sb = {0};
    char data [] = "helloworld";
    int silent = 0;
    int ret = lfs_fill_super(&sb, data, silent);
    ASSERT_EQ(0, ret);

}
