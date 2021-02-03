#include "gtest/gtest.h"

#include "Entry.h"

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
    ASSERT_EQ(32ul, entry.getSize());

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
