#include "Fat12BlockParser.h"

#include "FileDevice.h"
#include "Blocks.h"

#include <cassert>
#include <iostream>


int Fat12BlockParser::parseFsMeta(Blocks * blocks, size_t offset, size_t len){
    assert(len == 1);
    //1. auto blks = blocks.get(offset, len); 
    //2. details parse
    //3. create TextFsMeta
    return 0;
}
//TODO:
int Fat12BlockParser::parseFileAllocator(Blocks * blocks, size_t offset, size_t len){
    assert(len == 18);
    return 0;
}
//TODO:
int Fat12BlockParser::parseRootDirectory(Blocks * blocks, size_t offset, size_t len){
    assert(len == 14);
    return 0;
}
//TODO:
int Fat12BlockParser::parseData(Blocks * blocks, size_t offset, size_t len){
    assert(len == 2880-33);
    return 0;
}

void Fat12BlockParser::parse(Blocks * blocks){
    size_t offset = 0;
    parseFsMeta(_blocks, offset, 1); 

    offset += 1;
    parseFileAllocator(_blocks, offset, 18); 
    
    offset += 18;
    parseRootDirectory(_blocks, offset, 14); 

    offset += 14;
    parseData(_blocks, offset, blocks.size() - offset); 
}

void Fat12BlockParser::test(){
    std::string path("a.img");

    auto parser = new Fat12BlockParser;

    auto dev = FileDevice::loadFromImage(path);
    assert(dev);
    std::cout << dev->getBlocks()->toString() << std::endl;
    parser->parse(dev->getBlocks());

    delete dev;
    delete parser;
}
