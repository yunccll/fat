#include "FileAllocator.h"
#include <iostream>
#include <cassert>

FileAllocator::FileAllocator(uint32_t capacity){
    _clusterMap.resize(capacity);
}
FileAllocator::~FileAllocator(){
}

uint32_t FileAllocator::allocate(){
    std::vector<uint32_t>::iterator it = _clusterMap.begin();
    for( size_t i = 0; i < _clusterMap.size(); ++i){
        if(_clusterMap[i] == 0)
            return i;
    }
    return 0; //TODO:
}

void FileAllocator::free(uint32_t noCluster){
    _clusterMap[noCluster] = FileAllocator::EmptyCluster;
}

bool FileAllocator::isFree(uint32_t noCluster) const {
    return _clusterMap[noCluster] == EmptyCluster;
}
bool FileAllocator::isLastCluster(uint32_t noCluster) const{
    return _clusterMap[noCluster] == EndOfClusterChain;
}

uint32_t FileAllocator::getNextCluster(uint32_t noCluster){
    return 0;
}
void FileAllocator::setNextCluster(uint32_t noCluster, uint32_t nextCluster){
}

void FileAllocator::test(){
    FileAllocator fa(9*512*2/3);

    uint32_t noCluster = fa.allocate();
    assert(true == fa.isLastCluster(noCluster));
    assert(true == fa.isUsed(noCluster));

/*
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
