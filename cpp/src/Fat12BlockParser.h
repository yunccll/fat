#ifndef  FAT12BLOCKPARSER_H
#define  FAT12BLOCKPARSER_H

#include <cstddef>

class Blocks;

class Fat12BlockParser {
public:
    static void test();

    void parse(Blocks * blocks);

    int parseFsMeta(Blocks * blocks, size_t offset,  size_t len);
    int parseFileAllocator(Blocks * blocks, size_t offset, size_t len);
    int parseRootDirectory(Blocks * blocks, size_t offset, size_t len);
    int parseData(Blocks * blocks, size_t offset, size_t len);

private:
};


#endif   /* FAT12BLOCKPARSER_H */
