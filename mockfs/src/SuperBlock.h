
#ifndef  SUPERBLOCK_H
#define  SUPERBLOCK_H

#include<string>

class SuperBlock;

class FileSystem {
public:
    ~FileSystem(){} 

    virtual SuperBlock * createSuperBlock(void * dev) = 0;
    virtual void killSuperBlock(SuperBlock * sb) = 0;

private:
    std::string name;
    int flags;
};

class SuperBlock {
public:
    SuperBlock(FileSystem * fs)
    :fs(fs)
    {
    }
    ~SuperBlock(){}


    //allocInode();
    //destroyInode();
    //readInode();
    //writeInode();
    //dirtyInode();
    //putInode();
    //dropInode();
    //deleteInode();
    //
    //putSuperBlock();
    //writeSuperBlock();
    //
    //statFs();
    //clearInode();
    //showOptions();

private:
    const uint64_t blockSize;
    const uint64_t blockSizeBits;
    const uint64_t maxSizeOfFile;
    const uint64_t flags;
    const uint64_t magicNumber;

    bool isDirty;

    bool isSyncing;

    FileSystem * fs;

    // Device * device;(ptr + name)
    
    // File * files <list>
    
    // Inodes *inods; <list>
    // Inodes * dirtyInodes; <list>
    // Inodes * writingToDisk; <list>

    //void * extraInfo;
};

class Inode{
public:

private:

};


#endif   /* SUPERBLOCK_H */
