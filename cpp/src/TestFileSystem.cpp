#include "gtest/gtest.h"

#include "FileSystem.h"
#include "FsInfo.h"
#include "RootEntries.h"


#include "FileAllocator.h"

namespace fat {


class File {
public:
    File(const std::string & path, std::shared_ptr<FileSystem> fs)
    : path(path)
    , fs(fs){
    }
    virtual ~File(){
    }

    const std::string & getPath() const {
        return path;
    }
    std::shared_ptr<FileSystem> getFileSystem(){
        return fs;
    }
    virtual Status truncate(uint64_t size = 0) {
        //TODO: 
        return Status::OK(); 
    }
    virtual Status create(){
        //TODO: 
        return Status::OK();
    }

    virtual Status flush(){
        //TODO: 
        return Status::OK();
    }

    virtual Status findEntry(){
        return fs->findEntry(path, entry);
    }
    virtual Status getSize(uint64_t & size) const {
        if(entry != nullptr){
            size = entry->getFileSize();
            return Status::OK();
        }
        return Status::IOError("entry not set"); //TODO: Status::IOError(FindEntry First)
    }
    uint64_t getMaxBlockIndex() const {
        return entry->getFileSize() / getBlockBytes();
    }
    uint64_t getBlockBytes() const {
        return fs->getInfo()->bytesPerSector();
    }

    uint64_t getBlockIndex(uint64_t offset) const{
        return offset / getBlockBytes();
    }
    uint64_t getOffsetInBlock(uint64_t offset) const {
        return offset % getBlockBytes();
    }
    uint64_t getSize() const {
        return entry->getFileSize();
    }

    Status readBlock(uint64_t blockIndex, std::string & result, uint64_t * hintClustor){
        uint64_t cluster = entry->getFirstCluster();
        std::cout  << "first cluster:"  << cluster << std::endl;
        for(uint64_t i = 0; i < blockIndex; ++i){
            cluster = fs->getFileAllocator()->getNextCluster(cluster);
        }
        std::cout << "readBlock:" << cluster << std::endl;
        //fs->getDevice()->read(cluster+14, result);
        const char * buf = new char[512]{ 0x00, 0x01, 0x02, 0x3, 0x04, 0x05, 0x00};
        result.append(buf, 6);
        return Status::OK();
    }

private:
    std::string path;
    std::shared_ptr<Entry> entry;
    std::shared_ptr<FileSystem> fs;
};




class SequenceFileReader  {
public:
    SequenceFileReader(std::shared_ptr<File> file)
    : file(file) 
    , offset(0)
    , hintClustorNo(0)
    , isOpen(false)
    {
    }
    virtual ~SequenceFileReader(){
    }

    Status read(const uint64_t expectSize, std::string & result){
        if(!isOpen){
            return Status::InvalidArgument("not opened"); //TODO: Status::FileNotOpened
        }

        if(offset >= file->getSize()){
            return Status::EndOfFile();
        }
        const uint64_t oldOffset = offset;

        uint64_t leftSize = expectSize;
        while(offset < file->getSize() && leftSize > 0){
            uint64_t blockIndex = file->getBlockIndex(offset);

            std::string buf;
            auto s = file->readBlock(blockIndex, buf, &hintClustorNo);
            if(!s) { 
                offset = oldOffset;
                return s;
            }

            uint64_t offsetInBlock = file->getOffsetInBlock(offset);
            uint64_t len = std::min(leftSize, file->getBlockBytes()-offsetInBlock);
            result.append(buf, offsetInBlock, len);

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
    uint64_t hintClustorNo;
    bool isOpen;
};


/*   RandomFile
Status readPosition(uint64_t offset, const uint64_t expectSize, std::string & result){
    //TODO:
    return Status::OK();
}
//Seek
//seekBegin(10)
//seekEnd(-10)
//seek(+10), seek(-10)
*/

} //end of namespace fat



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

        ASSERT_TRUE(r->close().isOk());
        delete r;
    }

    //std::cout << fs->getInfo()->toString() << std::endl;
   
    //bool exist = false;
    //s = fs->isExist("/chenglun.txt", exist);
    //ASSERT_TRUE(s.isOk() && exist == false);


    s = fs->unmount();
    ASSERT_TRUE(s.isOk());
}

/*  
class public IoObject {
public:
    virtual ~IoObject(){}
    Status open() = 0;
    Status close() = 0;
};
class Reader {
public:
    Status read(const uint64_t expectSize, std::string result);
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
