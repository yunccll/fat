#ifndef  FSINFO_H
#define  FSINFO_H

#include <cstddef>
#include <cstdint>

#include <string>
#include <cstring>


namespace fat {

struct FsInfo {
public:
    FsInfo(){
        memset(this, 0, sizeof(FsInfo));
    }
    FsInfo(int64_t totalSector, int bytesPerSector, 
        int reservedSectorCount, int numberOfFats, 
        int sectorPerFat, int sectorPerCluster, 
        int rootEntryCount, int bytesPerEntry,
        unsigned char media
        );

    int64_t totalSector() const {
        return this->_totalSector;
    }
    int bytesPerSector() const{
        return this->_bytesPerSector;
    }
    int reservedSectorCount() const {
        return this->_reservedSectorCount;
    }
    int numberOfFats() const {
        return this->_numberOfFats;
    }
    int sectorPerFat() const {
        return _sectorPerFat;
    }
    int sectorPerCluster() const {
        return this->_sectorPerCluster;
    }
    int rootEntryCount() const {
        return this->_rootEntryCount;
    }
    int bytesPerEntry() const {
        return this->_bytesPerEntry;
    }
    int media() const {
        return this->_media;
    }

    int64_t totalBytes() const{
        return _totalSector * _bytesPerSector;
    }
    int64_t  numberOfDataSector() const{
        return _totalSector - ( _reservedSectorCount + _numberOfFats * _sectorPerFat + numberOfRootEntrySector());
    }
    int64_t totalClusters() const{
        return numberOfDataSector()/_sectorPerCluster;
    }

    int numberOfRootEntrySector() const{
        return (_rootEntryCount * _bytesPerEntry + (_bytesPerSector - 1))/_bytesPerSector;
    }

    int firstSectorOfMbr() const {
        return _firstSectorOfMbr;
    }
    int firstSectorOfFat() const {
        return _firstSectorOfFat;
    }
    int firstSectorOfFatBackup() const {
        return _firstSectorOfFatBackup;
    }
    int firstSectorOfRootEntry() const {
        return _firstSectorOfRootEntry;
    }
    int firstSectorOfData() const{
        return _firstSectorOfData;
    }
    int64_t sectorNumberOfcluster(int64_t clusterNo) const{
        return _firstSectorOfData + (clusterNo - 2);
    }
    int64_t clusterNoOfSector(int64_t sectorNo) const{
        return (sectorNo - _firstSectorOfData) + 2;
    }

    uint64_t dataSectorNoFromCluster(uint64_t clusterNo){
        return (clusterNo -2) * sectorPerCluster()  + firstSectorOfData();
    }

    std::string toString() const;

private:
    void calcFirstSectorOfAll() {
        _firstSectorOfMbr = 0;
        _firstSectorOfFat = firstSectorOfMbr() + reservedSectorCount();
        _firstSectorOfFatBackup = firstSectorOfFat() + sectorPerFat();
        _firstSectorOfRootEntry = firstSectorOfFatBackup() + sectorPerFat();
        _firstSectorOfData = firstSectorOfRootEntry() + numberOfRootEntrySector();
    }
private:
    int64_t _totalSector;
    int _bytesPerSector;        //512

    int _reservedSectorCount;    // 1 -> fat12/16  32 -> fat32
    int _numberOfFats;     // 2
    int _sectorPerFat;          // 0 -> fat32, 8 -> fat12,


    int _sectorPerCluster; // TODO: ??
    int _rootEntryCount;        // 0 --> fat32 512 --> fat16   220 --> fat12 1.44M
    int _bytesPerEntry;  //const 32

    int _media;// 0xF8 -> fixed media; 0xF0 --> removable media

    int _firstSectorOfMbr;
    int _firstSectorOfFat;
    int _firstSectorOfFatBackup;
    int _firstSectorOfRootEntry;
    int _firstSectorOfData;

};

class FsInfoBuilder {
public:
    static const int EntrySize = 32; //32bytes
    FsInfo * make();

    FsInfoBuilder();

    FsInfoBuilder & totalSector(int64_t totalSector){
        this->_totalSector = totalSector;
        return *this;
    }
    FsInfoBuilder & bytesPerSector(int bytesPerSector){
        this->_bytesPerSector = bytesPerSector;
        return *this;
    }
    FsInfoBuilder & reservedSectorCount(int reservedSectorCount ){
        this->_reservedSectorCount = reservedSectorCount;
        return *this;
    }
    FsInfoBuilder & numberOfFats(int numberOfFats){
        this->_numberOfFats = numberOfFats;
        return *this;
    }
    FsInfoBuilder  & sectorPerFat(int sectorPerFat){
        this->_sectorPerFat = sectorPerFat;
        return *this;
    }
    FsInfoBuilder & sectorPerCluster(int sectorPerCluster ){
        this->_sectorPerCluster = sectorPerCluster;
        return *this;
    }
    FsInfoBuilder & rootEntryCount(int rootEntryCount){
        this->_rootEntryCount = rootEntryCount;
        return *this;
    }
    FsInfoBuilder & bytesPerEntry(int bytesPerEntry ){
        this->_bytesPerEntry = bytesPerEntry;
        return *this;
    }
    FsInfoBuilder &  media(unsigned char media){
        this->_media = media;
        return *this;
    }


private:
    int64_t _totalSector;
    int _bytesPerSector;

    int _reservedSectorCount;
    int _numberOfFats;
    int _sectorPerFat;

    int _sectorPerCluster;

    int _rootEntryCount;
    int _bytesPerEntry;

    int _media;
};

} //end of namespace fat


#endif   /* FSINFO_H */
