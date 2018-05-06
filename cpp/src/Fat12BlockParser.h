#ifndef  FAT12BLOCKPARSER_H
#define  FAT12BLOCKPARSER_H


class Blocks;

class Fat12BlockParser {
public:
    static void test();

    void parse(Blocks * blocks);

    void parseFsMeta(Blocks * blocks, size_t offset,  size_t len);
    void parseFileAllocator(Blocks * blocks, size_t offset, size_t len);
    void parseRootDirectory(Blocks * blocks, size_t offset, size_t len);
    void parseData(Blocks * blocks, size_t offset, size_t len);

private:
    struct F12Meta {
    };
    struct F12FileAllocator{
    };
    struct F12RootDirector{
    }
};


#endif   /* FAT12BLOCKPARSER_H */
