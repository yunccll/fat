#ifndef  FAT12BLOCKPARSER_H
#define  FAT12BLOCKPARSER_H

#include <cstddef>
#include <cstdint>

#include <string>


namespace fat {

class FsInfo;
class Blocks;
class FileAllocator;
class BlockView;
struct dir;

class Fat12BlockParser {

    static const uint16_t BadCluster = 0x0ff7;
    static const uint16_t EndOfClusterChain = 0x0fff;
public:
    static void test();

    Fat12BlockParser();
    ~Fat12BlockParser();

    int parse(Blocks * blocks);

    int parseFsMeta(BlockView * bv);
    int parseFileAllocator(BlockView * bv);
    int parseRootDirectory(BlockView * bv);
    int parseData(Blocks * blocks, size_t offset, size_t len);


    //* ********************* */
    //Status parseMeta(const slice & block, FsInfo ** fsInfo);
    //Status parseFileAllocator(const slice & blocks, FileAllocator ** fileAllocator);

private:
    void printEntry(dir * entry, uint32_t head, BlockView * bv, int prefix);
    void visitInternalDirectory(BlockView * bv, int prefix);
private:
    FsInfo * _fsInfo;
    FileAllocator * _fileAllocator;
};

} //end of namespace fat

#endif   /* FAT12BLOCKPARSER_H */
