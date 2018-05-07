#include "Fat12BlockParser.h"

#include "FileDevice.h"
#include "Blocks.h"

#include <cassert>
#include <iostream>

#pragma pack(1)

struct BiosParameterBlock{
    uint16_t bytesPerSector;        //512
    unsigned char sectorPerCluster; // TODO: ??
    uint16_t reserveSectorCount;    // 1 -> fat12/16  32 -> fat32
    unsigned char numberOfFats;     // 2
    uint16_t rootEntryCount;        // 0 --> fat32 512 --> fat16   220 --> fat12 1.44M
    uint16_t totalSector16;         // must 0 --> FAT32,  real-count for fat12/16
    unsigned char media;            // 0xF8 -> fixed media; 0xF0 --> removable media
    uint16_t sectorPerFat;          // 0 -> fat32, 8 -> fat12,
    uint16_t sectorPerTrack;        // 
    uint16_t numberOfHeaders;       //
    uint32_t hiddenSectors;         // hidden sectors preceding this partition; 0 -> no partition
    uint32_t totalSector32;         //
};
struct BootSector{
    char jumpToBoot[3];
    char oemName[8];
    struct BiosParameterBlock bpb;
};
#pragma pack()


int Fat12BlockParser::parseFsMeta(Blocks * blocks, size_t offset, size_t len){
    assert(len == 1);
    BootSector * bootSector = (BootSector*)(blocks->getBlocks(0)->get());
    std::cout << bootSector->oemName << std::endl;
    //reserverRegion parse
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
    parseFsMeta(blocks, offset, 1); 

    offset += 1;
    parseFileAllocator(blocks, offset, 18); 
    
    offset += 18;
    parseRootDirectory(blocks, offset, 14); 

    offset += 14;
    parseData(blocks, offset, blocks->size() - offset); 
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
