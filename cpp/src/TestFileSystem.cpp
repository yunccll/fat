#include "gtest/gtest.h"

#include "FileSystem.h"
#include "File.h"


#include "Slice.h"
#include "Status.h"

namespace fat {

class SequenceFileReader  {
public:
    SequenceFileReader(std::shared_ptr<File> file)
    : file(file) 
    , offset(0)
    , isOpen(false)
    {
    }
    virtual ~SequenceFileReader(){
    }

    Status read(const uint64_t expectSize, std::string & result){
        if(!isOpen){
            return Status::FileNotOpen(file->getPath().c_str());
        }

        if(offset >= file->getSize()){
            return Status::EndOfFile();
        }
        const uint64_t oldOffset = offset;

        std::string blockBuf;
        blockBuf.reserve(file->getBlockBytes());

        uint64_t leftSize = expectSize;
        while(offset < file->getSize() && leftSize > 0){
            uint64_t blockIndex = file->getBlockIndex(offset);

            blockBuf.clear();
            auto s = file->readBlock(blockIndex, blockBuf); //read sector 
            if(!s) { 
                offset = oldOffset;
                return s;
            }
            uint64_t offsetInBlock = file->getOffsetInBlock(offset);
            uint64_t len = std::min(leftSize, file->getBlockBytes()-offsetInBlock);
            result.append(blockBuf, offsetInBlock, len); //copy to dest , perf optimal

            offset += len;
            leftSize -= len;
        }
        return Status::OK();
    }
    virtual Status open(){
        if(!isOpen){
            offset = 0;
            isOpen = true;
            return file->findEntry();
        }
        return Status::OK();
    }
    virtual Status close(){
        if(isOpen){
            isOpen = false;
            offset = 0;
        }
        return Status::OK();
    }
private:
    std::shared_ptr<File> file;
    uint64_t offset;
    bool isOpen;
};


class SequenceFileWriter  {
public:
    SequenceFileWriter(std::shared_ptr<File> file)
    : file(file) 
    , offset(0)
    , isOpen(false)
    {
    }
    virtual ~SequenceFileWriter(){
    }

    Status write(const Slice & data, size_t & outLen){
        //TODO: 
        return Status::OK();
    }

    virtual Status open(){
        if(!isOpen){
            isOpen = true;
            auto s = file->findEntry();
            if(!s) return s;
            offset = file->getSize();
            return s;
        }
        return Status::OK();
    }
    virtual Status close(){
        if(isOpen){
            isOpen = false;
            offset = 0;
        }
        return Status::OK();
    }
private:
    std::shared_ptr<File> file;
    uint64_t offset;
    bool isOpen;
};



} //end of namespace fat

using namespace fat;

TEST(FileSystemTest, use)
{
    //auto fs = std::make_shared<FileSystem>("floppy.img");
    auto fs = std::make_shared<FileSystem>("zero.img");
    auto s = fs->mount();
    ASSERT_TRUE(s.isOk());

    {
        std::string path("A.TXT");
        auto r = new SequenceFileReader(std::make_shared<File>(path ,fs));
        ASSERT_TRUE(r->open().isOk());

        std::string result;
        ASSERT_TRUE(r->read(5, result).isOk());
        ASSERT_EQ(5, result.size());

        while(r->read(10, result).isOk()) ;
        std::cout << "size: " << result.size() << " content:" << result << std::endl;

        ASSERT_TRUE(r->close().isOk());
        delete r;
    }
    {
        std::string path("B.TXT");
        auto w = new SequenceFileWriter(std::make_shared<File>(path, fs));
        ASSERT_TRUE(w->open().isOk());

        size_t outLen= 0;
        ASSERT_TRUE(w->write(Slice("hello"), outLen).isOk());
        ASSERT_TRUE(outLen == Slice("hello").size());
        
        ASSERT_TRUE(w->close().isOk());
        delete w;
    }

    //bool exist = false;
    //s = fs->isExist("/chenglun.txt", exist);
    //ASSERT_TRUE(s.isOk() && exist == false);

    s = fs->unmount();
    ASSERT_TRUE(s.isOk());
}

/*  
//   RandomFile
Status readPosition(uint64_t offset, const uint64_t expectSize, std::string & result){
    //TODO:
    return Status::OK();
}
//Seek
//seekBegin(10)
//seekEnd(-10)
//seek(+10), seek(-10)

class public IoObject {
public:
    virtual ~IoObject(){}
    Status open() = 0;
    Status close() = 0;
};
class RandomAccessObject : public IoObject {
public:
    //Seek
    //seekBegin(10)
    //seekEnd(-10)
    //seek(+10), seek(-10)
};

class FileOperationTest : public testing::Test {
public:
    FileOperationTest()
    :fs(new FileSystem("zero.img"))
    ,path("a.txt")
    {
        fs->mount();
    }
    ~FileOperationTest(){
        fs->unmount();
    }

    std::string path;
    std::shared_ptr<FileSystem> fs;
};

TEST_F(FileOperationTest, SequenceFileReader){
    auto r = new SequenceFileReader(std::make_shared<File>(path ,fs));
    ASSERT_TRUE(r->open().isOk());

    std::string result;
    ASSERT_TRUE(r->read(5, result).isOk());
    ASSERT_EQ(5, result.size());

    ASSERT_TRUE(r->close().isOk());
    delete r;
}

TEST_F(FileOperationTest, SequenceFileWriter){
    auto w = new SequenceFileWriter(std::make_shared<File>(path ,fs));
    ASSERT_TRUE(w->open().isOk());

    size_t outLen= 0;
    ASSERT_TRUE(w->write(Slice("hello", 5), outLen).isOk());
    ASSERT_EQ(5, outLen);

    ASSERT_TRUE(w->flush().isOk());

    ASSERT_TRUE(w->close().isOk());
    delete w;
}

TEST_F(FileOperationTest, RandomFileReader){
    auto r = new RandomFileReader(std::make_shared<File>(path ,fs));
    ASSERT_TRUE(r->open().isOk());

    //Position operations
    ASSERT_EQ(0, r->getPosition());
    r->seekFromCurrent(10);
    r->seekFromCurrent(-10);
    r->seekFromBegin(10);
    r->seekFromEnd(-10);

    std::string result;
    ASSERT_TRUE(r->read(5, result).isOk()); //with current position
    ASSERT_EQ(5, result.size());

    //
    result.clear();
    ASSERT_TRUE(5->readPosition(10, 5, result).isOk());
    ASSERT_EQ(5, result.size());

    ASSERT_TRUE(r->close().isOk());
    delete r;
}
//RandomFileWriter
*/
