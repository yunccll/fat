#include "Device.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>

namespace fat {


BlockDeviceFileImp::BlockDeviceFileImp(const std::string & name, const int blockSize)
: BlockDevice(name, blockSize) 
, fd(-1)
, readBuffer(new char[blockSize])
, writeBuffer(new char[blockSize])
{
    //Check blockSize is times of 512 (a sector)
}

BlockDeviceFileImp::~BlockDeviceFileImp() {
    if(writeBuffer != nullptr){
        delete [] writeBuffer;
    }
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
    // blockIndex -> offset
    uint64_t offset = offsetFrom(blockIndex);
    std::cout << "fd:" << fd  << " offset:" << offset << std::endl;
    // lseek
    int64_t ret = ::lseek(fd, offset, SEEK_SET);
    if(ret >= 0){
        // read it 
        ssize_t ret = ::read(fd, readBuffer, getBlockSize());
        return (ret >= 0) ? Status::OK() : Status::Failed("read file failed");
    }
    else{
        //TODO: get error_no & msg
        return Status::Failed("lseek failed");
    }
}

Status BlockDeviceFileImp::writeBlock(uint64_t blockIndex, const Slice & from){
    //TODO: 
    // blockIndex -> offset
    // lseek 
    // write it
    return Status::OK();
}

} //end of namespace fat
