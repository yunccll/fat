#include <iostream>

#include "FileDevice.h"
#include "Blocks.h"
#include "Fat12BlockParser.h"
#include "FileAllocator.h"

#include "gtest/gtest.h"
#include <memory>

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


/*  
TEST(FsTest, isExist){
    auto fs = std::make_shared<FileSystem>("chenglun.img");
    ASSERT_TRUE(fs->mount());
    ASSERT_FALSE(fs->isExist("/chenglun.txt"));
    ASSERT_TRUE(fs->unmount());
}

class FileTest : public Testing::Test {
public:
    FileTest()
    :fs(Fs::getDefaultFileSystem("chenglun.img")
    {
        fs->mount();
    }
    ~FileTest() override{
        fs->unmount();
    }
private:
    std::shared_ptr<Fs> fs;
};

TEST(FileTest, create){
    const char * newPath = "/chenglun.txt"
    std::shared_ptr<File> newFile(new File(newpath,fs)); //create it
    ASSERT_TRUE(newFile != nullptr);

    const char * oldPath = newPath;
    std::shared_ptr<file> oldFile(new File(oldPath, fs)); //open it 
    ASSERT_TRUE(oldFile != nullptr);
}




class FileWriterTest : public Testing::Test {
public:
    FileWriterTest()
    : fs(FileSystem::getDefault())
    , file(new File("/chenglun.txt")
    {
    }

    ~FileWriterTest()override {
        file->close();
    }

private:
    std::shared_ptr<FileSystem> fs;
    std::shared_ptr<File> file;
};
TEST(FileWriterTest, sequence){
    std::unique_ptr<SequenceFileWriter> w(new SequenceFileWriter(file));
    int ret = w->write("hello", 5);
    assert(ret >= 5);
    w->close();
}
TEST(FileWriterTest, random){
    std::unique_ptr<SequenceFileWriter> w(new RandomFileWriter(file));

    int ret = w.seek(10);
    ASSERT_TRUE(ret == 10);

    int ret = w->write("hello", 5);
    ASSERT_TRUE(ret >= 5);

    w->close();
}


class FileReaderTest : public Testing::Test {
public:
    FileReaderTest()
    : fs(FileSystem::getDefault())
    , file(new File("/chenglun.txt")
    {
    }

    ~FileReaderTest()override {
        file->close();
    }

private:
    std::shared_ptr<FileSystem> fs;
    std::shared_ptr<File> file;
};

TEST(FileReaderTest, sequence){
    const char * path = "/chenglun.txt";
    assert(fs->isExist(path) == false);

    std::unique_ptr<SequenceFileReader> r(new SequenceFileReader(std::shared_ptr<File>(path)));
    
    char buf[64] = {0};
    int ret = r->read(buf, sizeof(buf));
    assert(ret >= 0);

    r->close();
}
*/

/* 
int main(int argc, char * argv[]){
    std::cout << "hello world" << std::endl;
    Blocks::test();
    FileDevice::test();
    Fat12BlockParser::test();
    FileAllocator::test();
    return 0;
}
*/
