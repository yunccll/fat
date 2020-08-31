#include "gtest/gtest.h"

#include "File.h"
#include "FileSystem.h"

using namespace fat;

class FileTest : public testing::Test {
public:
    FileTest(){
        fs = std::make_shared<FileSystem>("zero.img");
        fs->mount();
    }
    ~FileTest(){
        fs->unmount();
    }

public:
    std::shared_ptr<FileSystem> fs;
};

TEST_F(FileTest, use){
    //TODO: setting an special file for this testcase ????
    std::string path("A.TXT");
    auto file = std::make_shared<File>(path, fs);
    file->findEntry();
    uint64_t offset = 0;
    //Move to File TestCase
    std::cout << "file Size: " << file->getSize()
        << " getBlockBytes:" << file->getBlockBytes() 
        << " maxBlockIndex:" << file->getMaxBlockIndex() 
        << " Block Index:" << file->getBlockIndex(offset)
        << " offset in Block:" << file->getOffsetInBlock(offset)
        << std::endl;
}
