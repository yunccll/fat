
#ifndef  BLOCKS_H
#define  BLOCKS_H

#include <string>
#include <vector>

class Block{
public:
    static const size_t BLOCK_SIZE = 512;

    Block(const char * buf, size_t len);
    ~Block();

    char * get(){return _buf;}

private:
    char _buf[BLOCK_SIZE];
};

class Blocks{

public:

public:
    Blocks();
    ~Blocks();

    void append(const char * buf, int len);
    size_t size() { return _blocks.size();}

    std::string toString();

    Block * getBlocks(size_t offset) {return  offset < size() ? _blocks[offset] : NULL; }

    static void test();
private:
    std::vector<Block * > _blocks;
};

#endif   /* BLOCKS_H */