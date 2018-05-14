#include <iostream>

#include "FileDevice.h"
#include "Blocks.h"
#include "Fat12BlockParser.h"
#include "FileAllocator.h"

int main(int argc, char * argv[]){
    std::cout << "hello world" << std::endl;
    Blocks::test();
    FileDevice::test();
    Fat12BlockParser::test();
    FileAllocator::test();
    return 0;
}
