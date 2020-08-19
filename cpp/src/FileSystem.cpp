#include "FileSystem.h"

#include "FsInfo.h"

namespace fat {

std::shared_ptr<FileSystem> FileSystem::__inst = nullptr;


FileSystem::FileSystem(const std::string & name)
: name(name)
{
}
FileSystem::~FileSystem(){
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
    //Device::Open();
    //1. load MBR 
    Status s = loadMeta();
    //TODO: 
    //2. load the fat_allocator
    //3. load the root directory //???
    return Status::OK();
}
Status FileSystem::unmount(){
    //TODO: 
    // flush root_dirctory & fat_allocator & MBR
    // free root_dirctory & fat_allocator & MBR
    return Status::OK();
}
Status FileSystem::stat(FsInfo & fsInfo){
    //TODO: 
    // Fill the FsInfo 
    return Status::OK();
}
Status FileSystem::loadMeta(){
    return Status::OK();
}


} //end of namespace fat
