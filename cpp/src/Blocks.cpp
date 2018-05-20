#include "Blocks.h"

#include <cstring>
#include <iostream>
#include <cassert>
#include <sstream>

Block::Block(const char * buf, size_t len)
{
    assert(len == BLOCK_SIZE);
    memcpy(_buf, buf, BLOCK_SIZE);
}
Block::~Block(){
}

uint16_t Block::getUint16(size_t bytesInBlock){
    return *((uint16_t *) (_buf + bytesInBlock));
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

void Blocks::test(){
    Blocks blks;
    char buffer[Block::BLOCK_SIZE] = {};
    blks.append(buffer, Block::BLOCK_SIZE);
    std::cout <<  "blocks 's size is :" << blks.size() << std::endl;
}

uint16_t Blocks::getUint16(size_t bytesOffset){
    size_t blockIndex = bytesOffset/Block::BLOCK_SIZE;
    size_t offsetInBlock = bytesOffset%Block::BLOCK_SIZE;
    return getBlock(blockIndex)->getUint16(offsetInBlock);
}
