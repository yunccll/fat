#include "FileDevice.h"
#include <iostream>
#include <fstream>

#include "Blocks.h"
using namespace std;

FileDevice::FileDevice(const std::string & path)
:_path(path)
,_blocks(NULL)
{
}

FileDevice::~FileDevice()
{
    if(_blocks){
        delete _blocks;
        _blocks = NULL;
    }
}

int FileDevice::readBlocks()
{
    std::ifstream in;
    in.open(_path, ios::in|ios::binary);
    if(!in){
        std::cout << "open file [" << _path << "] error!" << std::endl;
        return  -1;
    }

    _blocks = new Blocks;
    char buf[Block::BLOCK_SIZE];
    while(!in.eof()){
        in.read(buf, Block::BLOCK_SIZE);
        if(in)
            _blocks->append(buf, Block::BLOCK_SIZE);
    }
    in.close();
    return (int)_blocks->size();
}

FileDevice * FileDevice::loadFromImage(const std::string & path){
    FileDevice * ret = new FileDevice(path);
    return ret->readBlocks() > 0  ? ret : NULL;
}

void FileDevice::test(){
    const std::string path("main.bin");
    auto dev = FileDevice::loadFromImage(path);
    if(dev && dev->getBlocks()){
        std::cout << dev->getBlocks()->toString() << std::endl; 
    }
    else{
        std::cout << "read blocks failed" << std::endl;
    }
    if(dev){
        delete dev;
    }
}
