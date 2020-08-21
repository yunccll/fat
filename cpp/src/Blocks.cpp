#include "Blocks.h"

#include <cstring>
#include <iostream>
#include <cassert>
#include <sstream>

namespace fat {

Block::Block(const char * buf, size_t len)
{
    assert(len == BLOCK_SIZE);
    memcpy(_buf, buf, BLOCK_SIZE);
}
Block::~Block(){
}

uint16_t Block::getUint16(size_t bOffset){
    return *((uint16_t *) (_buf + bOffset));
}

uint8_t Block::getUint8(size_t bOffset){
    return uint8_t (*(_buf + bOffset));
}




Blocks::Blocks()
{
}
Blocks::~Blocks(){
    std::vector<Block *>::iterator it = _blocks.begin();
    for(; it != _blocks.end(); ++it){
        delete *it;
    }
}

void Blocks::append(const char * buf, int len){
    _blocks.push_back(new Block(buf, len));
}


std::string Blocks::toString(){
    std::ostringstream  ss;
    ss << "Blocks size:" << size();
    return ss.str();
}
BlockView * Blocks::getView(size_t blockIndex, size_t len){
    if(blockIndex < size()){
        if(blockIndex + len > size()){
            len = size() - blockIndex;
        }
        return new BlockView(this, blockIndex, len);
    }
    return NULL;
}

void Blocks::test(){
    Blocks blks;
    char buffer[Block::BLOCK_SIZE] = {};
    blks.append(buffer, Block::BLOCK_SIZE);
    std::cout <<  "blocks 's size is :" << blks.size() << std::endl;
}









BlockView::BlockView(Blocks * blocks, size_t blockIndex, size_t blockLen)
:_blocks(blocks)
,_blockIndex(blockIndex)
,_blockLen(blockLen)
{
}

BlockView::~BlockView(){
    _blockLen = 0;
    _blocks = NULL;
}

static inline Block * __getBlock(Blocks * blocks, size_t blockIndex, size_t bOffset){
    return blocks->getBlock(blockIndex + (bOffset / blocks->blockSize()));
}

uint16_t BlockView::getUint16(size_t bOffset){
    auto block = __getBlock(_blocks, _blockIndex, bOffset);
    assert(block);
    return block->getUint16(bOffset % _blocks->blockSize());
}
uint8_t BlockView::getUint8(int bOffset){
    auto block = __getBlock(_blocks, _blockIndex, bOffset);
    assert(block);
    return block->getUint8(bOffset % _blocks->blockSize());
}
char * BlockView::getBlockAddress(size_t blkOffset){
    auto block = _blocks->getBlock(_blockIndex + blkOffset);
    assert(block);
    return block->get();
}
char * BlockView::get(size_t bOffset){
    auto block = __getBlock(_blocks, _blockIndex, bOffset);
    assert(block);
    return block->get() + bOffset % _blocks->blockSize();
}

} //end of namespace fat
