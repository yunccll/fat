#ifndef  FILEDEVICE_H
#define  FILEDEVICE_H

#include <string>
class Blocks;

class FileDevice {
public:
    static void test();
    static FileDevice * loadFromImage(const std::string & path);


    FileDevice(const std::string & path);
    ~FileDevice();

    int readBlocks();

    Blocks * getBlocks(){
        return _blocks;
    }

private:
    std::string _path;
    Blocks * _blocks;
};

#endif   /* FILEDEVICE_H */
