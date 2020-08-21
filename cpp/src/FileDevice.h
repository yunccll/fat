#ifndef  FILEDEVICE_H
#define  FILEDEVICE_H

#include <string>

namespace fat {

class Blocks;

class FileDevice {
public:
    static void test();
    static FileDevice * loadFromImage(const std::string & path);


    FileDevice(const std::string & path);
    virtual ~FileDevice();


    int readBlocks();

    Blocks * getBlocks(){
        return _blocks;
    }
private:
    std::string _path;
    Blocks  * _blocks;
};

} //end of namespace fat

#endif   /* FILEDEVICE_H */
