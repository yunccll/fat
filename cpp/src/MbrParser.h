#pragma once


#include "Status.h"


namespace fat {

class Slice;
struct FsInfo;
class FileAllocator;

class MbrParser {
public:
    Status parse(const Slice & mbrBlock, FsInfo ** fsInfo);
};

class FileAllocatorParser {
public:
    static const uint16_t BadCluster = 0x0ff7;
    static const uint16_t EndOfClusterChain = 0x0fff; //>=
    Status parse(const Slice & blocks, const FsInfo * fsInfo, FileAllocator ** fa);
    Status build(FileAllocator * fa, std::string & result);
private:
    int32_t getClusterValue(FileAllocator * fa, size_t index);
};

} //end of namespace fat
