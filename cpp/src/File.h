#pragma once


#include <memory>
#include <string>
#include "Status.h"
#include "Slice.h"

namespace fat {

class FileSystem;
struct Entry;

class File {
public:
    File(const std::string & path, std::shared_ptr<FileSystem> fs);
    virtual ~File();

    const std::string & getPath() const {
        return path;
    }
    std::shared_ptr<FileSystem> getFileSystem(){
        return fs;
    }
    virtual Status truncate(uint64_t size = 0) ;
    virtual Status create();


    virtual Status findEntry();


    //assert findEntry OK at first
    virtual Status getSize(uint64_t & size) const ;

    uint64_t getMaxBlockIndex() const ;
    uint64_t getBlockBytes() const ;

    uint64_t getBlockIndex(uint64_t offset) const;
    uint64_t getOffsetInBlock(uint64_t offset) const ;
    uint64_t getSize() const ;

    Status readBlock(uint64_t blockIndex, std::string & result) const;


    Status readBlockWithCluster(uint64_t clusterNo, std::string & result);
    Status writeBlockWithCluster(uint64_t clusterNo, const Slice & data);


    uint64_t allocateCluster();
    void setNextCluster(int32_t noCluster, int32_t noNextCluster);
    void setLastCluster(int32_t noCluster);
    Status findLastCluster(uint64_t & clusterNo);
    void setFirstCluster(const uint64_t clusterNo);

    uint64_t getFirstCluster() const ;


    virtual void setSize(uint64_t size);
    virtual void updateWriteTimestamp();
    virtual void updateLastTimestamp();

    virtual Status flush();

private:
    std::string path;
    std::shared_ptr<Entry> entry;
    std::shared_ptr<FileSystem> fs;
    bool isDirty;
};

} //end of namespace fat
