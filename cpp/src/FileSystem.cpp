#include "FileSystem.h"

#include "Device.h"
#include "FsInfo.h"
#include "FileAllocator.h"
#include "MbrParser.h"

#include "Entry.h"
#include "RootEntries.h"

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
, rootArray(nullptr)
, rootUsedMap(new RootEntries)
{
}

FileSystem::~FileSystem(){
    delete rootUsedMap;
    delete rootArray;

    delete faParser;
    delete fa;

    delete mbrParser;
    delete fsInfo;

    delete device;
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
    //3. load the root directory
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
    size_t len = fsInfo->numberOfRootEntrySector();
    for(size_t i = 0; i < len; ++i){
        auto s = device->read((void*)(fsInfo->firstSectorOfRootEntry() + i), rootBuffer);
        if(!s) return s;
    }

    //std::cout << Slice(rootBuffer).ToString(true) << std::endl;
    rootArray = new RootEntryArray(rootBuffer.c_str(), rootBuffer.size());

    auto iter = rootArray->begin();
    for(; iter != rootArray->end(); ++iter){
        auto e = std::make_shared<Entry>(*iter);
        if(e->isFirstEmptyEntry()){ //0x00
            break;
        }
        else if(e->isFreeEntryAfterUse()){ //0xe5 
            continue;
        }
        else {
            std::string name;
            if(e->getName(name))
                rootUsedMap->add(name, e);
            else{
                //TODO: log it 
                continue;
            }

        }
    }
    std::cout << *rootUsedMap << std::endl;
    return Status::OK();
}

Status FileSystem::findEntry(const std::string & path ,std::shared_ptr<Entry> & entry){
    entry = rootUsedMap->getEntry(path);
    return entry != nullptr ? Status::OK() : Status::NotFound();
}
Status FileSystem::flushFat(){
    //TODO:
    fatBuffer[3] = 0x03;
    fatBuffer[4] = 0xf0;
    fatBuffer[5] = 0xff;

    for(int i = 0 ; i < 10; ++i){
        printf("0x%x, ", (unsigned char)(fatBuffer.c_str()[i]));
    }
    printf("\n");

    size_t len = fsInfo->sectorPerFat();
    for(size_t i = 0; i < len; ++i){
        auto s = device->write((void*)(fsInfo->firstSectorOfFat() + i), Slice(fatBuffer.c_str() + i * fsInfo->bytesPerSector(), fsInfo->bytesPerSector()));
        if(!s) return s;
    }
    return Status::OK();
}
Status FileSystem::flushRootDirectory(){
    size_t len = fsInfo->numberOfRootEntrySector();
    for(size_t i = 0; i < len; ++i){
        auto s = device->write((void*)(fsInfo->firstSectorOfRootEntry() + i), Slice(rootBuffer.c_str()+i * fsInfo->bytesPerSector(), fsInfo->bytesPerSector()));
        if(!s) return s;
    }
    return Status::OK();
}

} //end of namespace fat
