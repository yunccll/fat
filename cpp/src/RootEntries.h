#pragma once


#include <unordered_map>
#include <string>

#include "Slice.h"

namespace fat {

struct Entry {
public:
    Entry(const char * buf, size_t size)
    {
        assert(size >= sizeof(struct DirItem));
        memcpy(&item, buf, std::min(size, sizeof(struct DirItem)));
    }
    Entry(const Slice & directory)
    : Entry(directory.data(), directory.size())
    {
    }
    ~Entry(){
    }

    Slice getName() const {
        return Slice(item.name, 11);
    }

    
    uint64_t getEntrySize() const {
        return sizeof(struct DirItem);
    }

    static const uint8_t xFreeAfterUsed = 0xe5;
    bool isFreeEntryAfterUsed() const {
        return (item.name[0] == xFreeAfterUsed);
    }
    static const uint8_t xFreeEntry = 0x00;
    bool isFreeEntry() const {
        return (item.name[0] == xFreeEntry);
    }

    static const uint8_t xFakeKanJi = 0x05;
    bool isFakeKanJi() const {
        return (item.name[0] == xFakeKanJi);
    }

    static const uint8_t xSpace = 0x20;
    bool isSpace() const {
        return item.name[0] == xSpace;
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
        return (item.attribute & xLongName) == xLongName;
    }
    bool isDirectory() const {
        return (item.attribute & FaDirectory) == FaDirectory;
    }
    bool isReadOnly() const {
        return (item.attribute & FaReadOnly) == FaReadOnly;
    }
    bool isHidden() const {
        return (item.attribute & FaHidden) == FaHidden;
    }
    bool isSystem() const {
        return (item.attribute & FaSystem) == FaSystem;
    }
    uint8_t getAttribute() const {
        return item.attribute;
    }


    std::string getCreateTimeStamp() const;
    std::string getLastAccessDate() const;
    std::string getWriteTimeStamp() const;

    uint64_t getFirstCluster() const{
        return (((uint64_t)item.firstClusterHigh)<<16)||((uint64_t)item.firstClusterLow);
    }
    uint64_t getFileSize() const{
        return item.fileSize;
    }

private:
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
    DirItem item;
};


std::ostream & operator<< (std::ostream & os, const Entry & entry);

class RootEntries {
public:

    RootEntries(size_t capacity);
    ~RootEntries();

    typedef Entry * EntryPointer;

    bool add(const char* path, const EntryPointer entry);
    EntryPointer remove(const char * path);
    Entry *  getEntry(const char * path) const;
    EntryPointer modify(const char * path, const EntryPointer newEntry);

private:
    std::unordered_map<std::string, EntryPointer> maps;
};


} //end of namespace fat
