#include "FileAllocator.h"
#include <iostream>
#include <cassert>



namespace fat {

FileAllocator::FileAllocator(size_t capacity){
    _clusterMap.resize(capacity);
}
FileAllocator::~FileAllocator(){
}

int32_t FileAllocator::allocate(){
    for(int32_t i = 0; i < (int32_t)_clusterMap.size(); ++i){
        if(_clusterMap[i] == 0){
            return i;
        }
    }
    return -1;
}

void FileAllocator::free(int32_t noCluster){
    int32_t nextCluster = _clusterMap[noCluster];
    while( nextCluster != EndOfClusterChain && nextCluster != EmptyCluster){
        _clusterMap[noCluster] = EmptyCluster;
        noCluster = nextCluster;
        nextCluster = _clusterMap[noCluster];
    }
    _clusterMap[noCluster] = EmptyCluster;
}

bool FileAllocator::isFree(int32_t noCluster) const {
    assert(noCluster >= 0 && noCluster <= MaxCapacity);
    return _clusterMap[noCluster] == EmptyCluster;
}
bool FileAllocator::isLastCluster(int32_t noCluster) const{
    assert(noCluster >= 0 && noCluster <= MaxCapacity);
    return _clusterMap[noCluster] == EndOfClusterChain;
}
bool FileAllocator::isBadCluster(int32_t noCluster) const{
    assert(noCluster >= 0 && noCluster <= MaxCapacity);
    return _clusterMap[noCluster] == BadCluster;
}

int32_t FileAllocator::getNextCluster(int32_t noCluster) const{
    //std::cout << "noCluster:" << noCluster << std::endl;
    assert(noCluster >= 0 && noCluster <= MaxCapacity);

    return _clusterMap[noCluster];
}
void FileAllocator::setNextCluster(int32_t noCluster, int32_t noNextCluster){
    assert(noCluster >= 0 && noCluster <= MaxCapacity);
    _clusterMap[noCluster] = noNextCluster;
}
int32_t FileAllocator::getClusterValue(uint32_t clusterIndex) const{
    assert(clusterIndex >= 0 && clusterIndex < size());
    return _clusterMap[clusterIndex];
}

std::ostream  & operator<<(std::ostream & os, const FileAllocator & fa){
    const int max_print_count = 100;
    for(size_t i = 0; i < fa.size() && i < max_print_count; ++i){
       os << "i:" << i
            << "next cluster:"  << fa.getClusterValue(i)
            << std::endl;
    }
    return os;
}


void FileAllocator::testAlloc(){

    FileAllocator fa(9*512*2/3);
    fa.setNextCluster(0, FileAllocator::EndOfClusterChain);
    fa.setNextCluster(1, FileAllocator::EndOfClusterChain);

    {
        int32_t noCluster = fa.allocate();
        assert(noCluster != -1 && noCluster > 1);
        //std::cout << "allocated no cluster is: " << noCluster << std::endl;

        fa.setNextCluster(noCluster, EndOfClusterChain);
        assert(fa.isLastCluster(noCluster));
        assert(fa.getNextCluster(noCluster) == EndOfClusterChain);
        fa.free(noCluster);
    }

    {
        int32_t noCluster = fa.allocate();
        //fill the data in this cluster....
        fa.setNextCluster(noCluster, FileAllocator::EndOfClusterChain);

        int32_t noNextCluster = fa.allocate();
        //fill the data in this cluster ....
        fa.setNextCluster(noCluster, noNextCluster);
        fa.setNextCluster(noNextCluster, FileAllocator::EndOfClusterChain);

        assert(true == fa.isUsed(noNextCluster));
        assert(true == fa.isUsed(noCluster));

        fa.free(noCluster);
        assert(true == fa.isFree(noCluster));
        assert(true == fa.isFree(noNextCluster));
    }
}

void FileAllocator::test(){
    testAlloc();
}

} //end of namespace fat
