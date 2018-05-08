#include "FsInfo.h"
#include <sstream>

FsInfo::FsInfo(int64_t totalSector, int bytesPerSector, 
    int reservedSectorCount, int numberOfFats, 
    int sectorPerFat, int sectorPerCluster, 
    int rootEntryCount, int bytesPerEntry,
    unsigned char media
){
    this->_totalSector = totalSector;
    this->_bytesPerSector = bytesPerSector;
    this->_reservedSectorCount = reservedSectorCount;
    this->_numberOfFats = numberOfFats;
    this->_sectorPerFat = sectorPerFat;
    this->_sectorPerCluster = sectorPerCluster;
    this->_rootEntryCount = rootEntryCount;
    this->_bytesPerEntry = bytesPerEntry;
    this->_media = media;
}

std::string FsInfo::toString(){
    std::ostringstream  ss;
    ss << "FsInfo:"                 << std::endl;
    ss << "\ttotalBytes(K):"        << totalBytes()/1024 << std::endl;
    ss << "\tnumberOfDataSector:"   << numberOfDataSector() << std::endl;
    ss << "\ttotalSector:"          << _totalSector << std::endl;
    ss << "\tbytesPerSector:"       << _bytesPerSector << std::endl;
    ss << "\treservedSectorCount:"  << _reservedSectorCount << std::endl;
    ss << "\tnumberOfFats:"         << _numberOfFats << std::endl;
    ss << "\tsectorPerFat:"         << _sectorPerFat << std::endl;
    ss << "\tsectorPerCluster:"     << _sectorPerCluster << std::endl;
    ss << "\trootEntryCount:"       << _rootEntryCount << std::endl;
    ss << "\tmedia:0x"                << std::hex << _media << std::endl;
    return ss.str();
}


FsInfo * FsInfoBuilder::make(){
    return new FsInfo(_totalSector, _bytesPerSector, _reservedSectorCount,
    _numberOfFats, _sectorPerFat, _sectorPerCluster, _rootEntryCount, _bytesPerEntry,
    _media);
}

FsInfoBuilder::FsInfoBuilder(){
    this->_totalSector = 0;
    this->_bytesPerSector = 512;
    this->_reservedSectorCount = 1;
    this->_numberOfFats = 2;
    this->_sectorPerFat = 8;
    this->_sectorPerCluster = 2;
    this->_rootEntryCount = 220;
    this->_bytesPerEntry = FsInfoBuilder::EntrySize;
    this->_media = 0xF0;
}

