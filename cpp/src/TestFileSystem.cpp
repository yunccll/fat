#include "gtest/gtest.h"

#include "FileSystem.h"

using namespace fat;
TEST(FileSystemTest, use)
{
    auto fs = std::make_shared<FileSystem>("chenglun.img");
    auto s = fs->mount();
    ASSERT_TRUE(s.isOk());
    bool exist = false;
    s = fs->isExist("/chenglun.txt", exist);
    ASSERT_TRUE(s.isOk() && exist == false);

    s = fs->unmount();
    ASSERT_TRUE(s.isOk());
}

