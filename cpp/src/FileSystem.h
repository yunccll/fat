#pragma once

#include <memory>
#include <string>
#include "Status.h"


namespace fat {

class Device;
struct FsInfo;
class MbrParser;
class FileAllocator;
class FileAllocatorParser;
class RootEntryArray;
class RootEntries;
struct Entry;

class FileSystem {
    static const uint64_t MbrBlockIndex = 0;
public:
    FileSystem(const std::string & name);
    ~FileSystem();
    static std::shared_ptr<FileSystem> getDefaultFileSystem();

    Status isExist(const std::string & path, bool & exist);
    Status remove(const std::string & path);

    Status mount();
    Status unmount();

    FsInfo * getInfo() const {
        return fsInfo;
    }


    //just find in root directory
    Status findEntry(const std::string & path, std::shared_ptr<Entry> & entry);

private:
    Status loadMeta();
    Status loadFileAllocator();
    Status loadRootDirectory();


private:
    std::string name;
    Device * device;

    FsInfo * fsInfo;
    MbrParser * mbrParser;
    std::string mbr;

    FileAllocator * fa;
    FileAllocatorParser * faParser;
    std::string fatBuffer;


    RootEntryArray * rootArray;
    RootEntries * rootUsedMap;
    std::string rootBuffer;

    static std::shared_ptr<FileSystem> __inst;
};

} //end of namespace fat
