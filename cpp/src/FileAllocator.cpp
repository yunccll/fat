#include "FileAllocator.h"

FileAllocator::FileAllocator(){
}
FileAllocator::~FileAllocator(){
}

uint32_t FileAllocator::allocate(){
    return 0;
}

void FileAllocator::free(uint32_t noCluster){
}
bool FileAllocator::isFree() const {
    return true;
}
bool FileAllocator::isLastCluster() const{
    return true;
}

uint32_t FileAllocator::getNextCluster(uint32_t noCluster){
    return 0;
}
void FileAllocator::setNextCluster(uint32_t noCluster, uint32_t nextCluster){
}

void FileAllocator::test(){
    FileAllocator fa;

    uint32_t noCluster = fa.allocate();
/*
    assert(true == fa.isLastCluster(noCluster));
    assert(true == fa.isUsed(noCluster));

    uint32_t nextNoCluster = fa.allocate();
    assert(true == fa.isLastCluster(nextNoCluster));
    fa.setNextCluster(noCluster, nextNoCluster);
    assert(false == fa.isLastCluster(noCluster));
    assert(true == fa.isLastCluster(nextNoCluster));

    uint32_t tmp = fa.getNextCluster(noCluster);
    assert(tmp == nextNoCluster);

    fa.free(nextNoCluster);
    assert(true == fa.isFree(nextNoCluster));
    assert(true == fa.isFree(noCluster));
    */
    fa.free(noCluster);

    //TODO: fa.toBuffer(buffer);
    //TODO: fa = FileAllocator::fromBuffer()
}
