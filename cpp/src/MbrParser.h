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
    Status parse(const Slice & blocks, const FsInfo * fsInfo, FileAllocator ** fa);
};

} //end of namespace fat
