#include "FileSystem.h"


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
    return Status::OK();
}
Status FileSystem::mount(){
    return Status::OK();
}
Status FileSystem::unmount(){
    return Status::OK();
}
Status FileSystem::stat(){
    return Status::OK();
}

} //end of namespace fat
