#ifndef  FAT12BLOCKPARSER_H
#define  FAT12BLOCKPARSER_H

#include <cstddef>

class FsInfo;
class Blocks;
class FileAllocator;

class Fat12BlockParser {
public:
    static void test();

    Fat12BlockParser();
    ~Fat12BlockParser();

    int parse(Blocks * blocks);

    int parseFsMeta(Blocks * blocks, size_t offset,  size_t len);
    int parseFileAllocator(Blocks * blocks, size_t offset, size_t len);
    int parseRootDirectory(Blocks * blocks, size_t offset, size_t len);
    int parseData(Blocks * blocks, size_t offset, size_t len);

private:
    FsInfo * _fsInfo;
    FileAllocator * _fileAllocator;
};


#endif   /* FAT12BLOCKPARSER_H */
