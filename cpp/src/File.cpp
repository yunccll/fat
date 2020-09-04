#include "File.h"


#include "FileSystem.h"
#include "RootEntries.h"
#include "FsInfo.h"
#include "FileAllocator.h"
#include "Device.h"
#include "Entry.h"

#include <iostream>
#include <algorithm>

namespace fat {

File::File(const std::string & path, std::shared_ptr<FileSystem> fs)
: path(path)
, fs(fs)
, isDirty(false)
{

}

File::~File(){
}

Status File::truncate(uint64_t size) {
    //TODO: 
    return Status::OK(); 
}
Status File::create(){
    //TODO: 
    return Status::OK();
}

Status File::flush(){
    if(isDirty) {
        //data is flush when write
        auto s = fs->flushFat();
        if(!s) return s;
        return fs->flushRootDirectory();
    }
    return Status::OK();
}

Status File::findEntry(){
    return fs->findEntry(path, entry);
}

//assert findEntry OK at first
Status File::getSize(uint64_t & size) const {
    if(entry != nullptr){
        size = entry->getFileSize();
        return Status::OK();
    }
    return Status::IOError("entry not set"); //TODO: Status::IOError(FindEntry First)
}
uint64_t File::getMaxBlockIndex() const {
    return entry->getFileSize() / getBlockBytes();
}
uint64_t File::getBlockBytes() const {
    return fs->getInfo()->bytesPerSector();
}

uint64_t File::getBlockIndex(uint64_t offset) const{
    return offset / getBlockBytes();
}
uint64_t File::getOffsetInBlock(uint64_t offset) const {
    return offset % getBlockBytes();
}
//assert findEntry OK at first
uint64_t File::getSize() const {
    return entry->getFileSize();
}
void File::setSize(uint64_t size) {
    if(!isDirty) isDirty = true;
    entry->setFileSize(size);
}
void File::updateWriteTimestamp(){
    //TODO: 
    if(!isDirty) isDirty = true;
}
void File::updateLastTimestamp(){
    //TODO: 
    if(!isDirty) isDirty = true;
}


Status File::lastCluster(uint64_t & clusterNo){
    auto * fat = fs->getFileAllocator();
    clusterNo = entry->getFirstCluster();
    while(!fat->isLastCluster(clusterNo)){
        clusterNo = fat->getNextCluster(clusterNo);
    }
    return Status::OK();    
}

Status File::readBlock(uint64_t blockIndex, std::string & result) const{
    uint64_t clusterNo = entry->getFirstCluster();
    //std::cout  << "last read cluster:"  << clusterNo<< std::endl;
    for(uint64_t i = 0; i < blockIndex; ++i){
        clusterNo = fs->getFileAllocator()->getNextCluster(clusterNo);
    }
    //std::cout << "current read cluster:" << clustorNo << std::endl;
    uint64_t dataSector = fs->getInfo()->dataSectorNoFromCluster(clusterNo);
    return fs->getDevice()->read((void*)dataSector, result);
}


void File::setNextCluster(int32_t noCluster, int32_t noNextCluster){
    fs->getFileAllocator()->setNextCluster(noCluster, noNextCluster);
    if(!isDirty) isDirty = true;
}
void File::setLastCluster(int32_t noCluster){
    fs->getFileAllocator()->setLastCluster(noCluster);
    if(!isDirty) isDirty = true;
}

uint64_t File::allocateCluster(){
    return fs->getFileAllocator()->allocate();
}

uint64_t File::getFirstCluster() const {
    return entry->getFirstCluster();
}
Status File::readBlockWithCluster(uint64_t clusterNo, std::string & result){
    uint64_t dataSector = fs->getInfo()->dataSectorNoFromCluster(clusterNo);
    return fs->getDevice()->read((void*)dataSector, result);
}
Status File::writeBlockWithCluster(uint64_t clusterNo, const Slice & data){
    //std::cout << "clusterNo:" <<  clusterNo << ", data:" << data.ToString() << ", data:size" << data.size()  << std::endl;
    if(!isDirty) isDirty = true;
    uint64_t dataSector = fs->getInfo()->dataSectorNoFromCluster(clusterNo);
    //std::cout << " dataSector: " << dataSector << std::endl;
    return fs->getDevice()->write((void*)dataSector, data);
}

} //end of namespace fat

