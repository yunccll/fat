#pragma once

#include "Status.h"
#include "Slice.h"

namespace fat {

class Device {
public:
    const std::string & getName() const {
        return name;
    }
    void setName(const std::string & name){
        this->name = name;
    }

    virtual ~Device(){}

    virtual Status open() = 0;
    virtual Status close() = 0;

    virtual Status read(void * from, std::string & to) = 0;
    virtual Status write(void * to, const Slice & from) = 0;
    
protected:    
    Device(const std::string & name) : name(name) {}

private:
    std::string name;
};

class BlockDevice  : public Device {
public:
    BlockDevice(const std::string & name, const size_t blockSize)
    : Device(name)
    , blockSize(blockSize)
    {
    }

    ~BlockDevice()override {
    }

    const size_t getBlockSize() const {
        return blockSize;
    }

    Status read(void * from, std::string & to)override {
        return readBlock((uint64_t)from, to);
    }
    Status write(void * to, const Slice & from) override{
        return writeBlock((uint64_t)to, from);
    }

protected:
    virtual Status readBlock(uint64_t blockIndex, std::string & to) = 0;
    virtual Status writeBlock(uint64_t blockIndex, const Slice & from) = 0;

private:
    const size_t blockSize;
};


class BlockDeviceFileImp : public BlockDevice {
public:
    BlockDeviceFileImp(const std::string & name, const size_t blockSize = 512u);
    ~BlockDeviceFileImp() override ;

    Status open() override;
    Status close() override;

protected:
    uint64_t offsetFrom(uint64_t blockIndex);
    uint64_t blockIndexFrom(uint64_t offset);


    Status readBlock(uint64_t blockIndex, std::string & to) override;
    Status writeBlock(uint64_t blockIndex, const Slice & from) override;

private:
    int fd; 
    char * readBuffer;
};


} //end of namespace fat
