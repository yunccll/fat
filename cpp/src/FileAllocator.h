
#ifndef  FILEALLOCATOR_H
#define  FILEALLOCATOR_H


#include <vector>

class FileAllocator{
public:
    FileAllocator();
    ~FileAllocator();

    uint32_t allocate(); //TODO:
    void free(uint32_t noCluster); //TODO:

    bool isFree() const; //TODO:
    bool isUsed() const {return !isFree();}
    bool isLastCluster() const; //TODO:

    uint32_t getNextCluster(uint32_t noCluster); //TODO:
    void setNextCluster(uint32_t noCluster, uint32_t nextCluster); //TODO:

    static void test();
private:
    std::vector<uint32_t> _clusterMap;
};


#endif   /* FILEALLOCATOR_H */
