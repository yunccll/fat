#include "gtest/gtest.h"



namespace mockfs {
    
    class Entry {
    public:
    };
    class VfsMount{
    };

    class FsStruct {
    public:
    private:
        Entry * root;
        Entry * pwd;
        VfsMount * rootMnt;
        VfsMount * pwdMnt;
    };

    class FileStruct {
    public:
        int getFreeFile();
        freeFile(int fd);
        File * getFile(int fd);
        File * newFile();
    private:
        // atomic count;
        // file lock
        int maxFds; //current number of files
        File ** files;// array of files
        int nextFd;

        FdSet openFds; //bitmap
        int maxFdSet;

    };

    class File {
    public:
        //File operations
        int open();
        void close();
        int flush();
        uint64_t llseek(offset, origin);

        Dentry * readdir();
        int release();

    private:
        Dentry * entry;
        VfsMount * vfsmnt;
        uint32_t flags;
        uint32_t mode;
        uint64_t pos;
        // AddressSpace * addrSpace;
    }


    class Process {
    public:
        int open(const char * name){
            auto * f = files->getUnusedFile();
            Dentry * entry = nullptr;
            bool found = lookupDentry(name, &entry);
            if(!found){
               found = loadFromDisk(name, &entry);
               if(!found) return -INVALID_PATH;
            }
            f->setEntry(entry);
            return f->getfd();
        }
        void close(int fd){
            auto * f = files->getFile(fd);
            f->flush();
            f->close();
        }

        int read(int fd, char * buf, size_t len){
            auto * f = files->getFile(fd);
            return f->read(buf, len);
        }
        int write(int fd, const char * buf, size_t len){
            auto * f = files->getFile(fd);
            return f->write(buf, len);
        }

        int mount(){
            return 0;
        }
        void unmount(){
        }

        //TODO: all interfaces
        //ng
    private:
        FsStruct * fs;
        FileStruct * files;
    };



};


TEST(MainTest, use){
    mockfs::Process process;
    int fd = process.open("filename.txt");
    ASSERT_TRUE(fd >= 0);

    char buf[128] = {0};
    int ret = process.read(fd, buf, sizeof(buf));
    ASSERT_TRUE(ret >= 0);

    process.close(fd);
}
