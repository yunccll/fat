#include "gtest/gtest.h"

#include "FileSystem.h"
#include "File.h"
#include "FileAllocator.h"


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
            uint64_t len = std::min(leftSize, (uint64_t)file->getBlockBytes() - offsetInBlock);
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
    , curClusterNo(0)
    {
        block.reserve(file->getBlockBytes());
    }
    virtual ~SequenceFileWriter(){
    }

    //Move to utility
    bool isNotAlign(const uint64_t offset, const uint64_t blockSize) const {
        //assert(blockSize pow of 2)
        return offset & (blockSize - 1);
    }
    //Move to utility
    bool isAlign(const uint64_t offset, const uint64_t blockSize) const{
        return !isNotAlign(offset, blockSize);
    }
    uint64_t firstBlockSize(const uint64_t offset, const uint64_t blockSize) const {
        return blockSize - (offset & (blockSize-1));
    }

    Status writeFirstUnalignBlock(const char * src, size_t len, const uint64_t offsetInBlock,  const uint64_t clusterNo, size_t & outLen){
        block.clear();
        auto s = file->readBlockWithCluster(clusterNo, block);
        if(!s) return s;

        //copy data
        uint64_t writeBytes = std::min((uint64_t)len, file->getBlockBytes() - offsetInBlock);
        auto start = offsetInBlock;
        auto & thisBlock = block;
        std::for_each(src, src + writeBytes,  [&thisBlock, &start](char c) {
            thisBlock[start++] = c;
        });

        //write it
        s = file->writeBlockWithCluster(clusterNo, Slice(block));
        if(!s) return s;

        outLen += writeBytes;
        return s;
    }

    Status WriteAlignBlocks(const char * src, size_t len, uint64_t & firstClusterNo, uint64_t & lastClusterNo, size_t & outLen){
        uint64_t clusterNo = firstClusterNo;
        while(len > 0){
            block.clear();
            //insert data
            uint64_t writeBytes = std::min((uint64_t)len, file->getBlockBytes());
            std::copy(src, src + writeBytes, std::back_inserter(block));
            if(writeBytes < file->getBlockBytes())
                block.append(file->getBlockBytes() - writeBytes, '\0');

            //write last cluster data
            lastClusterNo = file->allocateCluster();
            auto s = file->writeBlockWithCluster(lastClusterNo, Slice(block));
            if(!s) return s;

            outLen += writeBytes;
            src += writeBytes;
            len -= writeBytes;
            
            file->setLastCluster(lastClusterNo); //fill the new AllocateCluster
            if(clusterNo == 0){
                firstClusterNo = lastClusterNo;
            }
            else{
                file->setNextCluster(clusterNo, lastClusterNo);
            }
            clusterNo = lastClusterNo;
        }
        return Status::OK();
    }

    Status writeInternal(const char * src, size_t len, const uint64_t offset, 
        uint64_t & firstClusterNo, uint64_t & lastClusterNo, size_t & outLen){
        assert(src != nullptr && len > 0);

        //handle first un-align block
        if(isNotAlign(offset, file->getBlockBytes())){
            auto s = writeFirstUnalignBlock(src, len, file->getOffsetInBlock(offset), firstClusterNo, outLen);
            if(!s)  return s;
        }

        std::cout << "Before Anglin Block:\n\t outLen :" << outLen
                << " firstClusterNo:" << firstClusterNo
                << " lastClusterNo:" << lastClusterNo
            << std::endl;

        assert(isAlign(offset+outLen, file->getBlockBytes()));
        //handle aligned next blocks
        return WriteAlignBlocks(src + outLen, len - outLen, firstClusterNo, lastClusterNo, outLen);
    }

    Status write(const Slice & data, size_t & outLen){
        if(data.size() == 0 || data.data() == nullptr){
            outLen = 0;
            return Status::OK();
        }


        uint64_t firstClusterNo = curClusterNo;
        uint64_t lastClusterNo  = 0;



        std::cout << "Before WriteInteral :\n\tdata size:"  << data.size() 
            << " offset:" << offset
            << " firstClusterNo:" << firstClusterNo
            << " lastClusterNo:" << lastClusterNo
            << " outLen:" << outLen
            << std::endl;

        auto s = writeInternal(data.data(), data.size(), offset, firstClusterNo, lastClusterNo, outLen); //fat update in this function
        if(!s) return s;

        std::cout << "After WriteInternal:\n\t outLen:" << outLen
            << " firstClusterNo:" << firstClusterNo
            << " lastClusterNo:" << lastClusterNo
            << std::endl;

        offset += outLen;
        curClusterNo = lastClusterNo;

        if(file->getSize() == 0){
            file->setFirstCluster(firstClusterNo);
        }
        file->setSize(offset);
        file->setLastCluster(lastClusterNo);
    
        //std::cout << "file Allocator:" << *(file->getFileSystem()->getFileAllocator()) << std::endl;

        return Status::OK();
    }

    virtual Status flush(){
        return file->flush();
    }

    virtual Status open(){
        if(!isOpen){
            isOpen = true;
            auto s = file->findEntry();
            if(!s) return s;

            offset = file->getSize();
            file->findLastCluster(curClusterNo);
            return s;
        }
        return Status::OK();
    }
    virtual Status close(){
        if(isOpen){
            isOpen = false;
            offset = 0;
            return file->flush();
        }
        return Status::OK();
    }
private:
    std::shared_ptr<File> file;
    uint64_t offset;
    bool isOpen;
    std::string block;
    uint64_t curClusterNo;
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
        //std::cout << "size: " << result.size() << " content:" << result << std::endl;

        ASSERT_TRUE(r->close().isOk());
        delete r;
    }
    {
        std::string path("A.TXT");
        auto w = new SequenceFileWriter(std::make_shared<File>(path, fs));
        ASSERT_TRUE(w->open().isOk());

        size_t outLen= 0;
        std::string str(513, 's');

        ASSERT_TRUE(w->write(Slice(str), outLen).isOk());
        ASSERT_TRUE(outLen == str.size());
        
        
        ASSERT_TRUE(w->close().isOk());
        delete w;
    }

    {
        std::string path("B.TXT");
        auto w = new SequenceFileWriter(std::make_shared<File>(path, fs));
        ASSERT_TRUE(w->open().isOk());

        size_t outLen= 0;
        std::string str(513, 's');

        ASSERT_TRUE(w->write(Slice(str), outLen).isOk());
        ASSERT_TRUE(outLen == str.size());
        
        
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
//Seek
//seekBegin(10)
//seekEnd(-10)
//seek(+10), seek(-10)


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
