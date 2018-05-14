#include "Fat12BlockParser.h"



#include <cassert>
#include <iostream>

#include "FileDevice.h"
#include "Blocks.h"
#include "FsInfo.h"
#include "FileAllocator.h"

Fat12BlockParser::Fat12BlockParser()
: _fsInfo(NULL)
, _fileAllocator(NULL)
{
}
Fat12BlockParser::~Fat12BlockParser(){
    if(_fileAllocator){
        delete _fileAllocator;
        _fileAllocator = NULL;
    }
    if(_fsInfo){
        delete _fsInfo;
        _fsInfo = NULL;
    }
}


#pragma pack(1)

struct Fat {
    unsigned char numberOfDriver;   // 00 -> floppy disk, 0x80 -> hard disks
    unsigned char reserved1;        // 0
    unsigned char bootSignature;    // 0x29 -> Extended boot signature 
    uint32_t volumeId;              //
    char volumeLabel[11];           //
    char fileSystemType[8];            // "FAT12   " "FAT16   " "FAT32   "
};

struct Fat32 {
    uint32_t sectorPerFAT32;      //only for Fat32
    uint16_t extendFlag;        //bits 0-3: zero-based number of active Fat, 4-6: reserved, 7: 0/1 8-15: reserved
    uint16_t fileSystemVersion; // 0:0
    uint32_t clusterNumberOfRootDirectory;  // 2
    uint16_t fsInfo;
    uint16_t backupBootSector;
    char reserved[12];
    unsigned char driverNumber;
    unsigned char reserved1;
};
struct BiosParameterBlock{
    uint16_t bytesPerSector;        //512
    unsigned char sectorPerCluster; // TODO: ??
    uint16_t reservedSectorCount;    // 1 -> fat12/16  32 -> fat32
    unsigned char numberOfFats;     // 2
    uint16_t rootEntryCount;        // 0 --> fat32 512 --> fat16   220 --> fat12 1.44M
    uint16_t totalSector16;         // must 0 --> FAT32,  real-count for fat12/16
    unsigned char media;            // 0xF8 -> fixed media; 0xF0 --> removable media
    uint16_t sectorPerFat;          // 0 -> fat32, 8 -> fat12,
    uint16_t sectorPerTrack;        // 
    uint16_t numberOfHeaders;       //
    uint32_t hiddenSectors;         // hidden sectors preceding this partition; 0 -> no partition
    uint32_t totalSector32;         //
    union fatSpecial {
        struct Fat fatBootSector;
        struct Fat32 fat32BootSector;
    } special;
};
struct BootSector{
    char jumpToBoot[3];
    char oemName[8];
    struct BiosParameterBlock bpb;
};
#pragma pack()


int Fat12BlockParser::parseFsMeta(Blocks * blocks, size_t offset, size_t len){
    BootSector * bootSector = (BootSector*)(blocks->getBlocks(0)->get());
    BiosParameterBlock * bpb = &(bootSector->bpb);

/*  
    std::cout << "totalSector16:" << (int)bpb->totalSector16 << std::endl;
    std::cout << "bytesPerSector:" << bpb->bytesPerSector << std::endl;
    std::cout << "reservedSectorCount:" << bpb->reservedSectorCount << std::endl;
    std::cout << "numberOfFats:" << (int)bpb->numberOfFats << std::endl;
    std::cout << "sectorPerFat:" << bpb->sectorPerFat << std::endl;
    std::cout << "sectorPerCluster:" << (int)bpb->sectorPerCluster << std::endl;
    std::cout << "rootEntryCount:" << bpb->rootEntryCount << std::endl;
    std::cout << "media:" << std::hex << (int)bpb->media << std::endl;
*/

    FsInfoBuilder builder;
    _fsInfo = builder.totalSector(bpb->totalSector16)
        .bytesPerSector(bpb->bytesPerSector)
        .reservedSectorCount(bpb->reservedSectorCount)
        .numberOfFats(bpb->numberOfFats)
        .sectorPerFat(bpb->sectorPerFat)
        .sectorPerCluster(bpb->sectorPerCluster)
        .rootEntryCount(bpb->rootEntryCount)
        .media(bpb->media)
        .make();
    std::cout << _fsInfo->toString() << std::endl;
    return 0;
}

int Fat12BlockParser::parseFileAllocator(Blocks * blocks, size_t offset, size_t len){
    assert(len == 18);
    _fileAllocator = new FileAllocator(len*2/3);
    
    _fileAllocator->setLastCluster(0);
    _fileAllocator->setLastCluster(1);

    /* TODO:
    for( int i = 2; i < _fsInfo->noOfCluster+2; ++i){
        _fileAllocator.setNextCluster(cluster(i), blocks.getUint16(cluster(i)));
    }
    */
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

int Fat12BlockParser::parse(Blocks * blocks){
    size_t offset = 0;
    parseFsMeta(blocks, offset, 1); 

    if(_fsInfo == NULL) return -1;
    offset += _fsInfo->reservedSectorCount();
    size_t fatLen = _fsInfo->numberOfFats() * _fsInfo->sectorPerFat();
    parseFileAllocator(blocks, offset, fatLen); 
    
    offset += fatLen;
    parseRootDirectory(blocks, offset, _fsInfo->numberOfRootEntrySector()); 

    offset += _fsInfo->numberOfRootEntrySector();
    parseData(blocks, offset, blocks->size() - offset); 
    return 0;
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
