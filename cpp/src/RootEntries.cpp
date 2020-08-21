#include "RootEntries.h"

#include <cassert>
#include <sstream>

namespace fat {

struct Date{
    uint16_t day : 5;
    uint16_t month : 4;
    uint16_t year : 7;
};
struct Time {
    uint16_t second : 5;
    uint16_t minute : 6;
    uint16_t hour   : 5;
};
static std::ostream & operator << (std::ostream & os, const Date & date){
    os << (uint64_t)(date.year + 1980) <<  "-"  << date.month << "-" << date.day;
    return os;
}
static std::ostream & operator << (std::ostream & os, const Time & time){
    os << time.hour << ":" << time.minute << ":" << time.second;
    return os;
}


std::string Entry::getCreateTimeStamp() const{
    std::ostringstream oss;
    oss << *reinterpret_cast<const Date*>(&(item.createDate));
    oss << " " ;
    oss << *reinterpret_cast<const Time*>(&(item.createTime));
    return oss.str();
}
std::string Entry::getLastAccessDate() const{ //read or write date
    std::ostringstream oss;
    oss << *reinterpret_cast<const Date*>(&(item.lastAccessDate));
    return oss.str();
}
std::string Entry::getWriteTimeStamp() const{
    std::ostringstream oss;
    oss << *reinterpret_cast<const Date*>(&(item.writeDate));
    oss << " ";
    oss << *reinterpret_cast<const Time*>(&(item.writeTime));
    return oss.str();
}

std::ostream & operator<< (std::ostream & os, const Entry & entry){
    //TODO: 
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


} //end of namespace fat
