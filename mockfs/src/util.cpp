#include <gtest/gtest.h>
#include "util.h"

#include <iostream>

TEST(UtilTest, isEvenOdd){
    ASSERT_TRUE(is_even(0));
    ASSERT_FALSE(is_odd(0));

    ASSERT_FALSE(is_even(1));
    ASSERT_TRUE(is_odd(1));

    ASSERT_TRUE(is_even(2));
    ASSERT_FALSE(is_odd(2));

    ASSERT_FALSE(is_even(3));
    ASSERT_TRUE(is_odd(3));
}

TEST(UtilTest, offset_of_cluster_even){
    ASSERT_EQ(0, offset_of_cluster_even(0));
    ASSERT_EQ(3, offset_of_cluster_even(2));
    ASSERT_EQ(6, offset_of_cluster_even(4));
}
TEST(UtilTest, offset_of_cluster_odd){
    ASSERT_EQ(1, offset_of_cluster_odd(1));
    ASSERT_EQ(4, offset_of_cluster_odd(3));
    ASSERT_EQ(7, offset_of_cluster_odd(5));
}

TEST(UtilTest, byte_array_get_uint16){

    uint8_t addr []  = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    ASSERT_EQ(0x412, byte_array_get_uint16_even(addr, 0));
    ASSERT_EQ(0x563, byte_array_get_uint16_odd(addr, 1));
    ASSERT_EQ(0x856, byte_array_get_uint16_even(addr, 2));
    ASSERT_EQ(0x9A7, byte_array_get_uint16_odd(addr, 3));
    ASSERT_EQ(0xC9A, byte_array_get_uint16_even(addr, 4));
}
