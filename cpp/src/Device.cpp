#include "Device.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include <iostream>

namespace fat {


BlockDeviceFileImp::BlockDeviceFileImp(const std::string & name, const size_t blockSize)
: BlockDevice(name, blockSize) 
, fd(-1)
, readBuffer(new char[blockSize])
{
    //Check blockSize is times of 512 (a sector)
}

BlockDeviceFileImp::~BlockDeviceFileImp() {
    if(readBuffer != nullptr){
        delete [] readBuffer;
    }
}

Status BlockDeviceFileImp::open() {
    fd = ::open(getName().c_str(), O_CREAT|O_RDWR,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
    return (fd >= 0) ? Status::OK() : Status::Failed("open file failed");
}
Status BlockDeviceFileImp::close(){
    return ::close(fd) == 0 ? Status::OK() : Status::Failed("close file failed");
}

uint64_t BlockDeviceFileImp::offsetFrom(uint64_t blockIndex){
    return blockIndex * getBlockSize();
}
#define IsAlign(x, y)   ((x)&((uint64_t)y))==0
uint64_t BlockDeviceFileImp::blockIndexFrom(uint64_t offset){
    assert(IsAlign(offset, getBlockSize()));
    return offset/getBlockSize();
}

Status BlockDeviceFileImp::readBlock(uint64_t blockIndex, std::string & to){
    uint64_t offset = offsetFrom(blockIndex);
    // lseek
    int64_t pos = ::lseek(fd, offset, SEEK_SET);
    if(pos >= 0){
        // read it 
        ssize_t bytes = ::read(fd, readBuffer, getBlockSize());
        if((size_t)bytes == getBlockSize()){
            to.append(readBuffer, bytes);
            return Status::OK();
        }
        return (bytes < 0) ? Status::IOErrorWithErrno("read from file failed") 
            : Status::IOError("Cann't read a block from file");
    }
    else{
        return Status::IOErrorWithErrno("lseek failed");
    }
}

Status BlockDeviceFileImp::writeBlock(uint64_t blockIndex, const Slice & from){
    assert(from.size() == getBlockSize());

    uint64_t offset = offsetFrom(blockIndex);
    // lseek 
    int64_t pos = ::lseek(fd, offset, SEEK_SET);
    if(pos >= 0){
        // write it
        ssize_t bytes = ::write(fd, from.data(), from.size());
        return (bytes>= 0 && ((size_t)bytes) == getBlockSize()) 
            ? Status::OK() 
            : Status::IOErrorWithErrno("write block to file failed!");
    }
    return Status::IOErrorWithErrno("lseek failed");
}

} //end of namespace fat
