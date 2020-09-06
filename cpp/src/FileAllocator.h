
#ifndef  FILEALLOCATOR_H
#define  FILEALLOCATOR_H

#include <cstdint>
#include <cstddef>
#include <vector>

// free, allocated, used, 

namespace fat {

class FileAllocator{

    /*  Move to Parser
    static const uint32_t Fat12EndOfClusterChain = 0x0ff8; //>=
    static const uint32_t Fat16EndOfClusterChain = 0x0fff8; //>=
    static const uint32_t Fat32EndOfClusterChain = 0x0ffffff8; //>=
    */

    static const int32_t MaxCapacity = 0x7fffffff;
    static const int32_t EndOfClusterChain = -1;
    static const int32_t BadCluster = -2;
    static const int32_t EmptyCluster = 0;
public:
    FileAllocator(size_t capacity);
    ~FileAllocator();

    int32_t allocate();
    void free(int32_t noCluster);

    bool isFree(int32_t noCluster) const;
    bool isUsed(int32_t noCluster) const {return !isFree(noCluster);}
    bool isLastCluster(int32_t noCluster) const;
    bool isBadCluster(int32_t noCluster) const;

    int32_t getNextCluster(int32_t noCluster);
    void setNextCluster(int32_t noCluster, int32_t noNextCluster);
    void setLastCluster(int32_t noCluster) {return setNextCluster(noCluster, EndOfClusterChain);}
    void setBadCluster(int32_t noCluster) {return setNextCluster(noCluster, BadCluster);}

    size_t size()const {return _clusterMap.size();}


    int32_t getClusterValue(uint32_t clusterIndex);

    static void test();
    static void testAlloc();

private:
    std::vector<int32_t> _clusterMap;
    //std::list<int32_t> _deletedClusters;
};

} //end of namespace fat

#endif   /* FILEALLOCATOR_H */
