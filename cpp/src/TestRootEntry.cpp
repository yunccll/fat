#include "gtest/gtest.h"

#include <iostream>

#include "RootEntries.h"
#include "Entry.h"

using namespace fat;
//prepare many entry for map Test
TEST(RootEntriesTest, use){
    RootEntries map;
}


class RootEntryArrayTest : public testing::Test {
public:
};
//prepare one rootEntry buffer for array r/w test


TEST_F(RootEntryArrayTest, readOneItem){
    uint8_t data[32] = {
            0x41, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
            0x54, 0x58, 0x54, 0x20, 0x00, 0x64, 0x3f, 0x0a, 
            0x3f, 0x4c, 0x3f, 0x4c, 0x00, 0x00, 0x3f, 0x0a,
            0x3f, 0x4c, 0x0b, 0x00, 0x0c, 0x00, 0x00, 0x00
    };

    RootEntryArray array(data, sizeof(data));
    ASSERT_EQ(1ul, array.getEntryCount());
    ASSERT_EQ(data,  (uint8_t*)&(array[0]));

    auto iter = array.begin();
    for(; iter != array.end(); ++iter){
        std::cout << Entry(*iter) << std::endl;
    }
}

TEST_F(RootEntryArrayTest, readMultiItem){
    uint8_t data[32*3] = {
        0x41, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x54, 0x58, 0x54, 0x20, 0x00, 0x64, 0x3f, 0x0a, 
        0x3f, 0x4c, 0x3f, 0x4c, 0x00, 0x00, 0x3f, 0x0a,
        0x3f, 0x4c, 0x0b, 0x00, 0x0c, 0x00, 0x00, 0x00,

        0xe5, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
        0x54, 0x58, 0x54, 0x20, 0x00, 0x64, 0x3f, 0x0a, 
        0x3f, 0x4c, 0x3f, 0x4c, 0x00, 0x00, 0x3f, 0x0a,
        0x3f, 0x4c, 0x0b, 0x00, 0x0c, 0x00, 0x00, 0x00,

        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    RootEntryArray array(data, sizeof(data));
    ASSERT_EQ(3ul, array.getEntryCount());
    ASSERT_EQ(data,  (uint8_t*)&(array[0]));

    auto iter = array.begin();
    for(; iter != array.end(); ++iter){
        Entry e(*iter);
        std::cout << e << std::endl;
    }
    for(size_t i =0 ; i< array.getEntryCount(); ++i){
        Entry e(array[i]);
        if(!e.isFirstEmptyEntry()){
            std::cout << Entry(array[i]) << std::endl;
        }
    }
}
