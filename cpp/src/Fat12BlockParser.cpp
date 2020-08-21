#include "Fat12BlockParser.h"



#include <cassert>
#include <iostream>

#include "FileDevice.h"
#include "Blocks.h"
#include "FsInfo.h"
#include "FileAllocator.h"

namespace fat {

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

struct dir{
    char  name[11];
    uint8_t attribute;
    uint8_t reserved;
    uint8_t createTimeMilisecondTenth;//milisecond
    uint16_t createTime;
    uint16_t createDate;
    uint16_t lastAccessDate;
    uint16_t firstClusterHigh;
    uint16_t writeTime;
    uint16_t writeDate;
    uint16_t firstClusterLow;
    uint32_t fileSize;
};


#pragma pack()



int Fat12BlockParser::parseFsMeta(BlockView  * bv){
    assert(bv->numberOfBlocks() == 1);

    BootSector * bootSector = (BootSector*)(bv->get(0));
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


static uint16_t getNextClusterNoFromFat(BlockView * bv, size_t clusterNo){
    uint16_t clusterVal = bv->getUint16(clusterNo + clusterNo/2);
    return (clusterNo & 0x01) ?  clusterVal >> 4 : clusterVal & 0xfff;
}

int Fat12BlockParser::parseFileAllocator(BlockView * bv){
    assert(bv->numberOfBlocks() == 18);

    std::cout << "parseFileAllocator:" << std::endl 
        << "\tfirst offset " << bv->bOffset() << "\t bytes length " << bv->bLength() << std::endl
        << "\tfirstSectorOfData " <<  _fsInfo->firstSectorOfData()  << std::endl
        << "\ttotalClusters:  " << _fsInfo->totalClusters() << std::endl;

    _fileAllocator = new FileAllocator(_fsInfo->totalClusters() + 2);
    std::cout << "\tfat12 allocator size(): " << _fileAllocator->size() << std::endl;

    _fileAllocator->setLastCluster(0);
    _fileAllocator->setLastCluster(1);

    size_t totalClusters =  _fsInfo->totalClusters();
    for(size_t clusterNo = 2; clusterNo < totalClusters + 2; ++clusterNo){

        uint16_t value = getNextClusterNoFromFat(bv, clusterNo);
        if(value < BadCluster){
            _fileAllocator->setNextCluster(clusterNo, value);
        }
        else if(value == BadCluster){
            _fileAllocator->setBadCluster(clusterNo);
        } 
        else {
            _fileAllocator->setLastCluster(clusterNo);
        }

        /*
           std::cout << std::dec << "clusterNo: " <<  clusterNo
           << std::hex << "\tfat[clusterNo]: 0x" << value 
           << "\tfileAllocator.getNextCluster: 0x" << _fileAllocator->getNextCluster(clusterNo)
           << std::endl;
           */
    }
    return 0;
}


static int __snprintfDate(char * dateBuf, size_t cap, uint16_t date){
    struct Date{
        uint16_t day : 5;
        uint16_t month : 4;
        uint16_t year : 7;
    };
    Date * d = (Date*)&date;
    return snprintf(dateBuf, cap, "%04d-%02d-%02d", d->year+1980, d->month, d->day);
}
static int __snprintfTime(char * timeBuf, size_t cap, uint16_t time){
    struct Time {
        uint16_t second : 5;
        uint16_t minute : 6;
        uint16_t hour   : 5;
    };
    Time * t = (Time*)&time;
    return snprintf(timeBuf, cap, "%02d:%02d:%02d", t->hour, t->minute, t->second* 2);
}

static void printEntryDetail(dir * entry){
        char crtDate[16]= {0};
        __snprintfDate(crtDate, sizeof(crtDate), entry->createDate);
        char crtTime[16] = {0};
        __snprintfTime(crtTime, sizeof(crtTime), entry->createTime);

        char wrtDate[16]= {0};
        __snprintfDate(wrtDate, sizeof(wrtDate), entry->writeDate);
        char wrtTime[16] = {0};
        __snprintfTime(wrtTime, sizeof(wrtTime), entry->writeTime);

        char acDate[16]= {0};
        __snprintfDate(acDate, sizeof(acDate), entry->lastAccessDate);

        printf("\tattr:%x s_nm:[%s] sz:[%u] clus:[%d] crt:[%s %s %x] wrt:[%s %s] last_ac:[%s]\n"
            , entry->attribute
            , entry->name, entry->fileSize, entry->firstClusterLow
            , crtDate, crtTime, entry->createTimeMilisecondTenth
            , wrtDate, wrtTime
            , acDate);

      //TODO: print the cluster chain info
}


void Fat12BlockParser::printEntry(dir * entry, uint32_t head, BlockView * bv, int prefix){
    if(entry->attribute == 0x0F){
        std::cout << "\tlong name";
        if( (0xf0 & head ) == 0x40){
            std::cout <<  "\tlast long entry, order:" << (head & 0x0f) << std::endl;
        }
        else{
            std::cout << "\torder: " << (head & 0x0f) << std::endl;
        }
    }
    else if( (((uint32_t)entry->attribute) & 0x10) == 0x10){//directory
        std::cout << "\tdirectory";
        printEntryDetail(entry);
        if( (uint32_t)(entry->name[0]) != 0x2e){
            BlockView * dirBv = bv->blocks()->getView(_fsInfo->sectorNumberOfcluster(entry->firstClusterLow), 1);
            visitInternalDirectory(dirBv, prefix + 1);
            delete dirBv;
        }
    }
    else{
        printEntryDetail(entry);
    }

}

void Fat12BlockParser::visitInternalDirectory(BlockView * bv, int prefix){
    size_t bytes = bv->bLength();
    for(size_t bOffset = 0; bOffset < bytes; bOffset += 32){
        dir * entry = (dir *)bv->get(bOffset);

        for(int i = 0 ; i < prefix; ++i){
            std::cout << "--";
        }
        uint32_t head = ((uint32_t)(entry->name[0]) & 0xff);
        std::cout << std::hex <<  head;

        if(head == 0x00) { //finished flag
            std::cout << std::endl;
            break;
        }
        else if(head == 0xe5){ // free after used
            std::cout << "\tfree entry" << std::endl;
        }
        else if(head == 0x05){
            std::cout << "name 0x05->0xE5";
            printEntry(entry, head, bv, prefix);
        }
        else if(head < 0x20){
            std::cout << "\terror   " << std::endl;
        }
        else {
            printEntry(entry, head, bv, prefix);
        }
        //std::cout << "\tname:" << std::string(entry->name, 11) << std::endl;

    }
}

int Fat12BlockParser::parseRootDirectory(BlockView * bv){
    assert(bv->numberOfBlocks() == 14);
    std::cout << "parseRootDirectory : " 
        << std::dec << "\tbytes: " << bv->bLength() 
        << "\tblocks: " << bv->numberOfBlocks() 
        << std::endl;
    visitInternalDirectory(bv, 0);
    return 0;
}
//TODO:
int Fat12BlockParser::parseData(Blocks * blocks, size_t offset, size_t len){
    assert(len == 2880-33);
    return 0;
}

int Fat12BlockParser::parse(Blocks * blocks){
    size_t offset = 0;

    {
        BlockView bv(blocks, offset, 1);
        parseFsMeta(&bv);
        offset += 1;
    }

    {
        if(_fsInfo == NULL){ 
            printf("Error: parse FsInfo failded\n");
            return -1;
        }
        size_t fatLen = _fsInfo->numberOfFats() * _fsInfo->sectorPerFat();
        BlockView bv(blocks, offset, fatLen);
        parseFileAllocator(&bv);
        offset += fatLen;
    }

    {
        BlockView bv(blocks, offset, _fsInfo->numberOfRootEntrySector()); 
        parseRootDirectory(&bv);
        offset += _fsInfo->numberOfRootEntrySector();
    }

    parseData(blocks, offset, blocks->size() - offset); 
    return 0;
}

void Fat12BlockParser::test(){
    std::string path("floppy.img");
    //std::string path("a.img");
    std::cout << "this image path is :     " <<  path << std::endl;

    auto parser = new Fat12BlockParser;

    auto dev = FileDevice::loadFromImage(path);
    assert(dev);
    std::cout << dev->getBlocks()->toString() << std::endl;
    parser->parse(dev->getBlocks());

    delete dev;
    delete parser;
}

} //end of namespace fat
