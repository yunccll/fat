#pragma once


#include <memory>
#include <string>
#include "Status.h"

namespace fat {

class FileSystem;
class Entry;

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

    virtual Status flush();

    virtual Status findEntry();

    //assert findEntry OK at first
    virtual Status getSize(uint64_t & size) const ;
    uint64_t getMaxBlockIndex() const ;
    uint64_t getBlockBytes() const ;

    uint64_t getBlockIndex(uint64_t offset) const;
    uint64_t getOffsetInBlock(uint64_t offset) const ;
    uint64_t getSize() const ;

    Status readBlock(uint64_t blockIndex, std::string & result) const;

private:
    std::string path;
    std::shared_ptr<Entry> entry;
    std::shared_ptr<FileSystem> fs;
};

} //end of namespace fat
