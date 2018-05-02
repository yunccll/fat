#include "BlockPrinter.h"



void test(){

    Blocks  * blks = new BlocksFat12Blocks;

    BlockPrinter * bp = new Fat12BlockPrinter;
    bp.setBlocks(blks){
        check(blks.length);
    }
    bp.print(std::cout & out){
        Fat12InfoPrinter(blks[0]).print(out);
        Fat12RootDirectoryPrinter(blks[18:20]).print(out);
        Fat12FileAllocatorPrinter(blks[1:9]).print(out);
    }

    delete bp;
    delete blks;
}
