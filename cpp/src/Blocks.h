
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

    size_t size(){return BLOCK_SIZE;}

    uint16_t getUint16(size_t bOffset);
    uint8_t getUint8(size_t bOffset);

private:
    char _buf[BLOCK_SIZE];
};


class BlockView;
class Blocks{
public:
    Blocks();
    ~Blocks();

    void append(const char * buf, int len);
    size_t size() { return _blocks.size();}

    std::string toString();

    Block * getBlock(size_t blockIndex) {return  blockIndex < size() ? _blocks[blockIndex] : NULL; }

    BlockView * getView(size_t index, size_t len);

    size_t blockSize() const {return Block::BLOCK_SIZE; }

    static void test();

private:
    std::vector<Block * > _blocks;
};

class BlockView {
public:
    BlockView(Blocks * blocks, size_t blockIndex, size_t blockLen);
    ~BlockView();

    char * get(size_t blkOffset);
    uint16_t getUint16(size_t bOffset);
    uint8_t getUint8(int bOffset);

    size_t indexInBlocks() const {return _blockIndex;}
    size_t numberOfBlocks() const {return _blockLen;}

    size_t bOffset() const  {return _blockIndex * _blocks->blockSize();}
    size_t bLength() const {return _blockLen * _blocks->blockSize(); }

    size_t size() const { return _blockLen; }
    size_t blockSize() const {return _blocks->blockSize(); }
    Blocks * blocks() {return _blocks; }

private:
    Blocks * _blocks;
    size_t _blockIndex;
    size_t _blockLen;
};

#endif   /* BLOCKS_H */
