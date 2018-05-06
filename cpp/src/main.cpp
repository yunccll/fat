#include <iostream>

#include "FileDevice.h"
#include "Blocks.h"
#include "Fat12BlockParser.h"

int main(int argc, char * argv[]){
    std::cout << "hello world" << std::endl;
    Blocks::test();
    FileDevice::test();
    Fat12BlockParser::test();
    return 0;
}
