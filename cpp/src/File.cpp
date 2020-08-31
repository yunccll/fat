#include "File.h"


#include "FileSystem.h"
#include "RootEntries.h"
#include "FsInfo.h"
#include "FileAllocator.h"
#include "Device.h"
#include "Entry.h"

namespace fat {

File::File(const std::string & path, std::shared_ptr<FileSystem> fs)
: path(path)
, fs(fs){
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
    //TODO: 
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

} //end of namespace fat
