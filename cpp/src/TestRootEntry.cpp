#include "gtest/gtest.h"

#include "RootEntries.h"

#include <iostream>

using namespace fat;
class EntryTest : public testing::Test {
public:
    EntryTest()
    {
        realEntry = new uint8_t[32] {
            0x41, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
            0x54, 0x58, 0x54, 0x20, 0x00, 0x64, 0x3f, 0x0a, 
            0x3f, 0x4c, 0x3f, 0x4c, 0x00, 0x00, 0x3f, 0x0a,
            0x3f, 0x4c, 0x0b, 0x00, 0x0c, 0x00, 0x00, 0x00
        }; 

        freeEntryAfterUse = new uint8_t[32] {
            0xe5, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
            0x54, 0x58, 0x54, 0x20, 0x00, 0x64, 0x3f, 0x0a, 
            0x3f, 0x4c, 0x3f, 0x4c, 0x00, 0x00, 0x3f, 0x0a,
            0x3f, 0x4c, 0x0b, 0x00, 0x0c, 0x00, 0x00, 0x00
        };
        emptyEntry = new uint8_t[32] {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

    }
    ~EntryTest(){
        delete [] realEntry;
        delete [] freeEntryAfterUse;
        delete [] emptyEntry;
    }

protected:    
    uint8_t * realEntry;
    uint8_t * freeEntryAfterUse;
    uint8_t * emptyEntry;
};

//prepare one entry buffer
TEST_F(EntryTest, realEntry){
    Entry entry(realEntry, 32);
    ASSERT_EQ(std::string("A       TXT"), entry.getStorageName().ToString());
    ASSERT_EQ(32, entry.getSize());

    std::string name;
    ASSERT_TRUE(entry.getName(name));
    ASSERT_EQ(std::string("A.TXT"), name);


    ASSERT_TRUE(entry.isValidEntry());
    ASSERT_FALSE(entry.isFreeEntry());
    ASSERT_FALSE(entry.isFreeEntryAfterUse());
    ASSERT_FALSE(entry.isFirstEmptyEntry());

    ASSERT_FALSE(entry.isLongName());
    ASSERT_FALSE(entry.isDirectory());
    ASSERT_FALSE(entry.isReadOnly());
    ASSERT_FALSE(entry.isHidden());
    ASSERT_FALSE(entry.isSystem());
    ASSERT_TRUE(entry.isArchive());

    ASSERT_EQ(0x20, entry.getAttribute());
    std::cout << entry << std::endl;
}
TEST_F(EntryTest, freeEntries){
    {
        Entry entry(freeEntryAfterUse, 32);
        ASSERT_FALSE(entry.isValidEntry());
        ASSERT_TRUE(entry.isFreeEntry());
        ASSERT_TRUE(entry.isFreeEntryAfterUse());
        ASSERT_FALSE(entry.isFirstEmptyEntry());
    }
    {
        Entry entry(emptyEntry, 32);
        ASSERT_FALSE(entry.isValidEntry());
        ASSERT_TRUE(entry.isFreeEntry());
        ASSERT_FALSE(entry.isFreeEntryAfterUse());
        ASSERT_TRUE(entry.isFirstEmptyEntry());
    }
}


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
    ASSERT_EQ(1, array.getEntryCount());
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
    ASSERT_EQ(3, array.getEntryCount());
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