#pragma once


#include <string>
#include <memory>
#include <unordered_map>

#include "Slice.h"

namespace fat {

#pragma pack(1)
struct DirItem{
    char  name[11];
    uint8_t attribute;
    uint8_t reserved;
    uint8_t createTimeMilisecondTenth;//milisecond
    uint16_t createTime;
    uint16_t createDate;
    uint16_t lastAccessDate;
    uint16_t firstClusterHigh;
    uint16_t writeTime;
    uint16_t writeDate;
    uint16_t firstClusterLow;
    uint32_t fileSize;

};
#pragma pack()

struct Entry {
public:
    Entry(DirItem & dirItem)
    :item(&dirItem)
    {
        assert(sizeof(dirItem) == EntrySize());
    }
    Entry(uint8_t * buf, size_t size)
    :item(reinterpret_cast<DirItem*>(buf))
    {
        assert(buf != nullptr);
        assert(size == EntrySize());
    }
    ~Entry(){
    }

    Slice getName() const {
        return Slice(item->name, 11);
    }
    static uint64_t EntrySize() {
        return sizeof(struct DirItem);
    }
    uint64_t getSize() const{
        return Entry::EntrySize();
    }

    static const uint8_t xFreeAfterUsed = 0xe5;
    bool isFreeEntryAfterUse() const {
        return (uint8_t(item->name[0]) == xFreeAfterUsed);
    }
    static const uint8_t xEmptyEntry = 0x00;
    bool isFirstEmptyEntry() const {
        return (uint8_t(item->name[0]) == xEmptyEntry);
    }
    bool isFreeEntry() const {
        return isFreeEntryAfterUse() || isFirstEmptyEntry();
    }
    bool isValidEntry() const {
        return !isFreeEntry();
    }

    static const uint8_t xFakeKanJi = 0x05;
    bool isFakeKanJi() const {
        return (uint8_t(item->name[0]) == xFakeKanJi);
    }

    static const uint8_t xSpace = 0x20;
    bool isSpace() const {
        return uint8_t(item->name[0]) == xSpace;
    }
    //  any of name[i] invalid 
    //   1. values less 0x20 except 0x05, 0x00
    //   2. 0x22, 0x2A, 0x2B, 0x2C, 0x2E, 0x2F, 
    //      0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 
    //      0x5B, 0x5C, 0x5D, and 0x7C
    
    //* *********attribute*******/ 
    enum FileAttrbite {
        FaReadOnly    = 0x01,
        FaHidden      = 0x02,
        FaSystem      = 0x04,
        FaVolumnId    = 0x08,
        FaDirectory   = 0x10,
        FaArchive     = 0x20,
    };
    static const uint8_t xLongName = FaReadOnly|FaHidden|FaSystem|FaVolumnId;
    bool isLongName() const {
        return (item->attribute & xLongName) == xLongName;
    }
    bool isDirectory() const {
        return (item->attribute & FaDirectory) == FaDirectory;
    }
    bool isReadOnly() const {
        return (item->attribute & FaReadOnly) == FaReadOnly;
    }
    bool isHidden() const {
        return (item->attribute & FaHidden) == FaHidden;
    }
    bool isSystem() const {
        return (item->attribute & FaSystem) == FaSystem;
    }
    bool isArchive() const {
        return (item->attribute & FaArchive) == FaArchive;
    }
    uint8_t getAttribute() const {
        return item->attribute;
    }


    std::string getCreateTimeStamp() const;
    std::string getLastAccessDate() const;
    std::string getWriteTimeStamp() const;

    uint64_t getFirstCluster() const{
        return (((uint64_t)item->firstClusterHigh)<<16)||((uint64_t)item->firstClusterLow);
    }
    uint64_t getFileSize() const{
        return item->fileSize;
    }

private:
    DirItem * item;
};
std::ostream & operator<< (std::ostream & os, const Entry & entry);



class RootEntries {
public:
    typedef std::shared_ptr<Entry> EntryPointer;

    bool add(const char* path, const EntryPointer entry);
    EntryPointer remove(const char * path);
    EntryPointer  getEntry(const char * path) const;
    EntryPointer modify(const char * path, const EntryPointer newEntry);

    std::ostream & operator<<(std::ostream & os) const;
private:
    std::unordered_map<std::string, EntryPointer> maps;
};

std::ostream & operator<< (std::ostream & os, const RootEntries & );


class RootEntryArray {
public:
    RootEntryArray(const char * data, size_t len)
    : data((uint8_t*)(const_cast<char*>(data)))
    , len(len)
    {
    }
    RootEntryArray(char * data, size_t len)
    : data((uint8_t*)data)
    , len(len)
    {
    }
    RootEntryArray(uint8_t * data, size_t len)
    : data(data)
    , len(len)
    {
        assert(data != nullptr);
        assert(len > 0 && (len % Entry::EntrySize()) == 0);
    }
    ~RootEntryArray(){
    }

    //attribute 
    size_t getEntryCount() const{
        return len / Entry::EntrySize();
    }

    typedef DirItem value_type;
    typedef value_type& reference;
    typedef value_type* pointer;

    const reference operator[](size_t index) const {
        size_t offset  = index * Entry::EntrySize();
        assert(offset >=0 && offset < len);
        return *reinterpret_cast<pointer>(data+offset);
    }
    //read/write
    reference operator[](size_t index){
        size_t offset  = index * Entry::EntrySize();
        assert(offset >=0 && offset < len);
        return *reinterpret_cast<pointer>(data + offset);
    }

    class iterator {
    public:
        iterator(uint8_t * data, size_t index, size_t step)
        :data(data)
        ,index(index)
        ,step(step)
        {
        }
        ~iterator(){}


        typedef iterator self_type;
        typedef DirItem value_type;
        typedef value_type& reference;
        typedef value_type* pointer;

        //++iterator
        iterator & operator++(){
            index += step;
            return *this;
        }
        //iterator++
        iterator operator++(int){
            auto old = *this;
            index += step;
            return old;
        }
        //--iterator
        iterator & operator--(){
            index -= step;
            return *this;
        }
        //iterator--
        iterator operator--(int){
            auto old = *this;
            index -= step;
            return old;
        }
        reference operator *(){
            return *(reinterpret_cast<pointer>(data + index));
        }
        pointer operator->(){
            return reinterpret_cast<pointer>(data+index);
        }
        bool operator ==(const iterator & iter){
            return data == iter.data && index == iter.index;
        }
        bool operator !=(const iterator & iter){
            return ! (*this==iter);
        }

    private:
        uint8_t * data;
        size_t index;
        size_t step;
    };
    //range read 
    iterator begin() {
        return iterator(data, 0, Entry::EntrySize());
    }
    iterator end(){
        return iterator(data, len, Entry::EntrySize());
    }

    //TODO: write
private:
    uint8_t * data;
    size_t len;
};


} //end of namespace fat
