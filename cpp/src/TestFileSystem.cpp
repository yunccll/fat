#include "gtest/gtest.h"

#include "FileSystem.h"
#include "FsInfo.h"

using namespace fat;
TEST(FileSystemTest, use)
{
    auto fs = std::make_shared<FileSystem>("floppy.img");
    auto s = fs->mount();
    ASSERT_TRUE(s.isOk());

    std::cout << fs->getInfo()->toString() << std::endl;

    bool exist = false;
    s = fs->isExist("/chenglun.txt", exist);
    ASSERT_TRUE(s.isOk() && exist == false);

    s = fs->unmount();
    ASSERT_TRUE(s.isOk());
}

