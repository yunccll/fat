#include "FileSystem.h"

#include "Device.h"
#include "FsInfo.h"
#include "FileAllocator.h"
#include "MbrParser.h"

#include <iostream>

namespace fat {

std::shared_ptr<FileSystem> FileSystem::__inst = nullptr;


FileSystem::FileSystem(const std::string & name)
: name(name)
, device(new BlockDeviceFileImp(name))
, fsInfo(nullptr)
, mbrParser(new MbrParser())
, fa(nullptr)
, faParser(nullptr)
{
}

FileSystem::~FileSystem(){
    if(faParser != nullptr){
        delete faParser;
    }
    if(fa != nullptr){
        delete fa;
    }
    if(mbrParser != nullptr){
        delete mbrParser;
    }
    if(fsInfo != nullptr){
        delete fsInfo;
    }
    if(device != nullptr){
        delete device;
    }
}
std::shared_ptr<FileSystem> FileSystem::getDefaultFileSystem(){
    if(__inst != nullptr){
        __inst.reset(new FileSystem("chenglun.img"));
    }
    return __inst;
}

Status FileSystem::isExist(const std::string & path, bool & exist){
    //TODO: 
    // 1. parse the path  to read the file exist ???
    // 2. read the inode
    return Status::OK();
}
Status FileSystem::mount(){
    Status s = device->open();
    if(!s) return s;
    //1. load MBR 
    s = loadMeta();
    if(!s) return s;
    //2. load the fat_allocator
    s = loadFileAllocator();
    if(!s) return s;
    //3. load the root directory //???
    return loadRootDirectory();
}

Status FileSystem::unmount(){
    Status s;
    //TODO: 
    // flush root_dirctory & fat_allocator & MBR
    // free root_dirctory & fat_allocator & MBR
    s = device->close();
    if(!s) return s;
    return Status::OK();
}

Status FileSystem::loadMeta(){
    auto s = device->read((void*)MbrBlockIndex, mbr);
    if(s){
        return mbrParser->parse(Slice(mbr), &fsInfo);
    }
    return s;
}
/*
Status FileSystem::flushMeta(){
    auto s = mbrParser->build(fsInfo, mbr);
    if(!s) return s;
    return device->write((void*)MbrBlockIndex, Slice(mbr));
}*/

Status FileSystem::loadFileAllocator(){
    size_t fatLen = fsInfo->sectorPerFat();

    for(size_t i = 0; i < fatLen; ++i){
        auto s = device->read((void*)(fsInfo->firstSectorOfFat() +i), fatBuffer);
        if(!s) return s;
    }
    return faParser->parse(Slice(fatBuffer), fsInfo, &fa);
}

Status FileSystem::loadRootDirectory(){
    //TODO: 
    return Status::OK();
}

} //end of namespace fat
