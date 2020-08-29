#include "gtest/gtest.h"

#include "FileSystem.h"
#include "FsInfo.h"
#include "RootEntries.h"

namespace fat {


class File {
public:
    File(const std::string & path, std::shared_ptr<FileSystem> fs)
    : path(path)
    , position(0)
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
    virtual Status open() {
        auto s = fs->findEntry(path, entry);
        if(s){
            position = 0;
        }
        return s;
    }
    /*
    virtual Status seek(uint64_t newPos){
        uint64_t size;
        auto s = getSize(size);
        if(!s) return s;

        if(newPos < size){
            position = newPos;
            return Status::OK();
        }
        return Status::InvalidArgument("newPos >= size");
    }*/
    virtual Status close() {
        if(isOpen()){
            //TODO: return entry->flush(); //Data flush , Meta Flush
            position = 0;
        }
        return Status::OK();
    }
    virtual Status getSize(uint64_t & size) const {
        if(isOpen()){
            size = entry->getFileSize();
            return Status::OK();
        }
        return Status::InvalidArgument("file not opened"); //TODO: Status::FileNotOpened();
    }
    virtual bool isOpen() const{
        return entry != nullptr;
    }

    //Sectors , offset, len
    virtual Status read(uint64_t offset, const uint64_t expectSize, std::string & result){
        // offset to numbersOfSector
        // numbersOfSector validation
        // read sectors
        // return Slice(sectors, start, end)
        return Status::OK();
    }
    virtual Status write(uint64_t offset, const Slice & data, uint64_t & outLen){
        return Status::OK();
    }

private:
    std::string path;
    uint64_t position;
    std::shared_ptr<Entry> entry;
    std::shared_ptr<FileSystem> fs;
};




class SequenceFileReader  {
public:
    SequenceFileReader(std::shared_ptr<File> file)
    : file(file) 
    {
    }
    virtual ~SequenceFileReader(){
    }

    Status read(const uint64_t expectSize, std::string result){
        //TODO:
        return Status::OK();
    }
    virtual Status open(){
        return file->open();
    }
    virtual Status close(){
        return file->close();
    }

private:
    //uint64_t position;
    std::shared_ptr<File> file;
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
TEST(FileSystemTest, use)
{
    //auto fs = std::make_shared<FileSystem>("floppy.img");
    auto fs = std::make_shared<FileSystem>("zero.img");
    auto s = fs->mount();
    ASSERT_TRUE(s.isOk());

    /*  
    {
        std::string path("a.txt");
        auto r = new SequenceFileReader(std::make_shared<File>(path ,fs));
        ASSERT_TRUE(r->open().isOk());

        std::string result;
        ASSERT_TRUE(r->read(5, result).isOk());
        ASSERT_EQ(5, result.size());

        ASSERT_TRUE(r->close().isOk());
        delete r;
    }*/

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

class FileOperationTest : public testing::File {
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
