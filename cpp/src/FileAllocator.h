
#ifndef  FILEALLOCATOR_H
#define  FILEALLOCATOR_H

#include <cstdint>
#include <vector>

class FileAllocator{

    /*  Move to Parser
    static const uint32_t Fat12EndOfClusterChain = 0x0ff8; //>=
    static const uint32_t Fat16EndOfClusterChain = 0x0fff8; //>=
    static const uint32_t Fat32EndOfClusterChain = 0x0ffffff8; //>=
    */


    static const uint32_t EndOfClusterChain = (uint32_t)-1;
    static const uint32_t EmptyCluster = 0;
public:
    FileAllocator(uint32_t capacity);
    ~FileAllocator();

    uint32_t allocate();
    void free(uint32_t noCluster);

    bool isFree(uint32_t noCluster) const;
    bool isUsed(uint32_t noCluster) const {return !isFree(noCluster);}
    bool isLastCluster(uint32_t noCluster) const;

    uint32_t getNextCluster(uint32_t noCluster); //TODO:
    void setNextCluster(uint32_t noCluster, uint32_t nextCluster); //TODO:


    uint32_t size()const {return _clusterMap.size();}

    static void test();

private:
    std::vector<uint32_t> _clusterMap;
    //std::list<uint32_t> _deletedClusters;
};


#endif   /* FILEALLOCATOR_H */
