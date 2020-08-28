#include "RootEntries.h"

#include <cassert>
#include <sstream>
#include <iostream>

namespace fat {

void dateOut(std::ostream & os, uint16_t date){
    os << (date>>9) + 1980  << "-" << std::max(1, (date >> 5) & 0xf) << "-" << std::max(1, date & 0x1f) - 1;
}
void timeOut(std::ostream & os, uint16_t time){
    os << (time >> 11)  << ":" << ((time >> 5) & 0x3f) << ":" <<  ((time & 0x1f) << 1);
}
Status Entry::getName(std::string & name) const{ //for short entry(8~3)
    //FileName
    int i = 0;
    while(i < 8 && item->name[i] != (char)0x20) ++i;
    if(i == 0) {
        return Status::InvalidArgument(); //Invalid entry name
    }
    name.assign(item->name, i);

    //Suffix
    i = 0;
    while(i < 3 && item->name[8+i] != (char)0x20) ++i;
    if(i == 0)
        return Status::InvalidArgument(); //Invalid entry suffix
    name.append(".");
    name.append(item->name+8, i);
    return Status::OK();
}
std::string Entry::getCreateTimeStamp() const{
    std::ostringstream oss;
    dateOut(oss, item->createDate);
    oss << " " ;
    timeOut(oss, item->createTime);
    return oss.str();
}
std::string Entry::getLastAccessDate() const{ //read or write date
    std::ostringstream oss;
    dateOut(oss, item->lastAccessDate);
    return oss.str();
}
std::string Entry::getWriteTimeStamp() const{
    std::ostringstream oss;
    dateOut(oss, item->writeDate);
    oss << " ";
    timeOut(oss, item->writeTime);
    return oss.str();
}

std::ostream & operator<< (std::ostream & os, const Entry & entry){
    os << "Name:["              << entry.getStorageName().ToString() 
       << "] Attrbute:["        << (int)entry.getAttribute() 
       << "] CreateTimestamp:[" << entry.getCreateTimeStamp() 
       << "] lastAccessDate:["  << entry.getLastAccessDate() 
       << "] writeTimestamp:["  << entry.getWriteTimeStamp() 
       << "] firstCluster:["    << entry.getFirstCluster()
       << "] fileSize:["        << entry.getFileSize()
       << "]";
    return os;
}

bool RootEntries::add(const char * path, const RootEntries::EntryPointer entry){
    assert(path != nullptr);
    auto iter = maps.find(path);
    if(iter == maps.end()){
        maps.insert(std::make_pair(path, entry));
        return true;
    }
    return false;
}
RootEntries::EntryPointer RootEntries::remove(const char * path){
    auto iter = maps.find(path);
    if(iter != maps.end()){
        maps.erase(iter);
        return iter->second;
    }
    return nullptr;
}

RootEntries::EntryPointer RootEntries::getEntry(const char * path) const{
    auto iter = maps.find(path);
    if(iter != maps.end()){
        return iter->second;
    }
    return nullptr;
}
RootEntries::EntryPointer RootEntries::modify(const char * path, const RootEntries::EntryPointer newEntry){
    auto iter = maps.find(path);
    if(iter != maps.end()){
        auto old = iter->second;
        iter->second = newEntry;
        return old;
    }
    return nullptr;
}

bool RootEntries::add(const std::string & path, const EntryPointer entry){
    auto iter = maps.find(path);
    if(iter == maps.end()){
        maps.insert(std::make_pair(path, entry));
        return true;
    }
    return false;
}
RootEntries::EntryPointer RootEntries::remove(const std::string & path){
    auto iter = maps.find(path);
    if(iter != maps.end()){
        maps.erase(iter);
        return iter->second;
    }
    return nullptr;
}
RootEntries::EntryPointer RootEntries::getEntry(const std::string & path) const{
    auto iter = maps.find(path);
    if(iter != maps.end()){
        return iter->second;
    }
    return nullptr;
}
RootEntries::EntryPointer RootEntries::modify(const std::string & path, const EntryPointer newEntry){
    auto iter = maps.find(path);
    if(iter != maps.end()){
        auto old = iter->second;
        iter->second = newEntry;
        return old;
    }
    return nullptr;
}

std::ostream & RootEntries::operator<<(std::ostream & os) const{
    for(auto & pair : maps){
        os << "name:[" << pair.first << "], entry:{" << *(pair.second) << "}" << std::endl;
    }
    return os;
}
std::ostream & operator<< (std::ostream & os, const RootEntries & rootEntries){
    rootEntries.operator<<(os);
    return os;
}


} //end of namespace fat
