#include "util.h"


int is_even(sector_t cluster_no){  //0 2
    return (cluster_no & 0x01) == 0x00;
}
int is_odd(sector_t cluster_no){ //1 3
    return (cluster_no & 0x01) == 0x01;
}
uint64_t offset_of_cluster_even(sector_t cluster_no){
    assert(is_even(cluster_no));
    return (cluster_no * 3) >> 1;
}
uint16_t byte_array_get_uint16_even(uint8_t * addr, uint64_t offset){
    assert(addr != NULL);
    return *(uint16_t*)(addr + offset) & 0xfff;
}
void byte_array_set_uint16_even(uint8_t *  addr, uint64_t offset, uint64_t val){
    *(uint16_t*)(addr+ offset) = ((uint16_t)(val & 0xfff)) | (*(uint16_t*)(addr + offset) & 0xf000);
}
uint64_t offset_of_cluster_odd(sector_t cluster_no){
    assert(is_odd(cluster_no));
    return (((cluster_no - 1) *3 ) >> 1) + 1;
}
uint16_t byte_array_get_uint16_odd(uint8_t * addr, uint64_t offset){
    assert(addr != NULL);
    return *(uint16_t*)(addr + offset) >> 4;
}
void byte_array_set_uint16_odd(uint8_t *  addr, uint64_t offset, uint64_t val){
    *(uint16_t*)(addr+ offset) = ((uint16_t(val&0xfff)) << 4) | (*((uint16_t*)(addr + offset)) & 0x0f);
}

#include <gtest/gtest.h>
#include <iostream>


TEST(UtilTest, isEvenOdd){
    ASSERT_EQ(1, is_even(0));
    ASSERT_EQ(0, is_odd(0));

    ASSERT_EQ(0, is_even(1));
    ASSERT_EQ(1, is_odd(1));

    ASSERT_EQ(1, is_even(2));
    ASSERT_EQ(0, is_odd(2));

    ASSERT_EQ(0, is_even(3));
    ASSERT_EQ(1, is_odd(3));
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

TEST(UtilTest, byte_array_set_uint16){
    uint8_t addr []  = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    byte_array_set_uint16_even(addr, 0, 0x123);
    ASSERT_EQ(0x123, byte_array_get_uint16_even(addr, 0));
    byte_array_set_uint16_odd(addr, 1, 0x456);
    byte_array_set_uint16_even(addr, 2, 0x789);
    byte_array_set_uint16_odd(addr, 3, 0xabc);
    byte_array_set_uint16_even(addr, 4, 0xdef);
}
