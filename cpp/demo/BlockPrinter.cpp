#include "BlockPrinter.h"

void test(){
    Blocks  * blks = new BlocksFat12Blocks;

    FsStringBuilder builder = new FsStringBuilder();
    Fat12Parser parser = new Fat12Parser(builder)
    parser.parse(blks);
    FsString fs = builder.getResult()
    std::cout << fs.toString() << std::endl;

    delete blks;
}
