#include "MbrParser.h"
#include <cassert>
#include <iostream>

#include "Slice.h"

#include "FsInfo.h"
#include "FileAllocator.h"



namespace fat {

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

Status MbrParser::parse(const Slice & mbrBlock, FsInfo ** fsInfo){
    assert(fsInfo != nullptr);

    BootSector * bootSector = (BootSector*)(mbrBlock.data());
    BiosParameterBlock * bpb = &(bootSector->bpb);

    FsInfoBuilder builder;
    *fsInfo = builder.totalSector(bpb->totalSector16)
        .bytesPerSector(bpb->bytesPerSector)
        .reservedSectorCount(bpb->reservedSectorCount)
        .numberOfFats(bpb->numberOfFats)
        .sectorPerFat(bpb->sectorPerFat)
        .sectorPerCluster(bpb->sectorPerCluster)
        .rootEntryCount(bpb->rootEntryCount)
        .media(bpb->media)
        .make();
    return Status::OK();
}

static uint16_t getNextClusterNoFromFat(const Slice & blocks, size_t clusterNo){
    uint16_t clusterVal = *(uint16_t*)(blocks.data() + clusterNo + clusterNo/2);
    return (clusterNo & 0x01) ?  clusterVal >> 4 : clusterVal & 0xfff;
}

Status FileAllocatorParser::parse(const Slice & blocks, const FsInfo * fsInfo, FileAllocator ** fa){
    assert(fa != nullptr);

    *fa = new FileAllocator(fsInfo->totalClusters() + 2);
//    std::cout << "\tfat12 allocator size(): " << (*fa)->size()
//        << fsInfo->toString() 
//        << std::endl;
    

    (*fa)->setLastCluster(0);
    (*fa)->setLastCluster(1);

    size_t totalClusters =  fsInfo->totalClusters();
    for(size_t clusterNo = 2; clusterNo < totalClusters + 2; ++clusterNo){

        uint16_t value = getNextClusterNoFromFat(blocks, clusterNo);
        if(value < BadCluster){
            (*fa)->setNextCluster(clusterNo, value);
        }
        else if(value == BadCluster){
            (*fa)->setBadCluster(clusterNo);
        } 
        else {
            (*fa)->setLastCluster(clusterNo);
        }
//           std::cout << std::dec << "clusterNo: " <<  clusterNo
//           << std::hex << "\tfat[clusterNo]: 0x" << value 
//           << "\tfileAllocator.getNextCluster: 0x" << (*fa)->getNextCluster(clusterNo)
//           << std::endl;
    }
    return Status::OK();
}


int32_t FileAllocatorParser::getClusterValue(FileAllocator * fa, size_t index){
    if(fa->isLastCluster(index)){
        return EndOfClusterChain;
    }
    else if(fa->isBadCluster(index)){
        return BadCluster;
    }
    else{
        return fa->getClusterValue(index);
    }
}


//TODO: need more test  ???????
Status FileAllocatorParser::build(FileAllocator * fa, std::string & result){
    assert(result.size() > fa->size() * 3 / 2 + 1);

    size_t len = fa->size();
    for(size_t i = 0; i < len; ++i){
        int32_t clusterNo = getClusterValue(fa, i);
        uint64_t offset = ((i & (~0x1)) >> 1)*3;
        if( (i & 0x01) == 0x00){ //Even
            result[offset] = clusterNo&0xff;
            result[offset+1] =  ((uint8_t)(result[offset+1]) | 0x0f) & (((clusterNo >> 16) & 0x0f) | 0xf0);
        }
        else{  //Odd
            result[offset+1] = ((uint8_t)(result[offset+1]) | 0xf0) & (( ((clusterNo) & 0x0f) << 4) | 0x0f);
            result[offset+2] = (clusterNo >> 4) & 0xff;
        }
        std::cout << std::hex << " i:" << i
            << " clusterNo:" << clusterNo
            << " offset:" << offset
            << " result:" << ((unsigned short)result[offset] & 0xff) <<  ", "  << ((unsigned short)result[offset+1] &0xff) << ", " << ((unsigned short)result[offset+2] & 0xff)
            << std::endl;
    }
    return Status::OK(); 
}

} //end of namespace fat
