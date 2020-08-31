#include "RootEntries.h"

#include "Entry.h"

namespace fat {

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



RootEntryArray::RootEntryArray(const char * data, size_t len)
: data((uint8_t*)(const_cast<char*>(data)))
, len(len)
{
}
RootEntryArray::RootEntryArray(char * data, size_t len)
: data((uint8_t*)data)
, len(len)
{
}
RootEntryArray::RootEntryArray(uint8_t * data, size_t len)
: data(data)
, len(len)
{
    assert(data != nullptr);
    assert(len > 0 && (len % Entry::EntrySize()) == 0);
}
RootEntryArray::~RootEntryArray(){
}

//attribute 
size_t RootEntryArray::getEntryCount() const{
    return len / Entry::EntrySize();
}

RootEntryArray::reference RootEntryArray::operator[](size_t index) const {
    size_t offset  = index * Entry::EntrySize();
    assert(offset >=0 && offset < len);
    return *reinterpret_cast<pointer>(data+offset);
}
//read/write
RootEntryArray::reference RootEntryArray::operator[](size_t index){
    size_t offset  = index * Entry::EntrySize();
    assert(offset >=0 && offset < len);
    return *reinterpret_cast<pointer>(data + offset);
}

//range read 
RootEntryArray::iterator RootEntryArray::begin() {
    return iterator(data, 0, Entry::EntrySize());
}
RootEntryArray::iterator RootEntryArray::end(){
    return iterator(data, len, Entry::EntrySize());
}


} //end of namespace fat
