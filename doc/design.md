

super-block
file system control block

boot-loader
fat1-fat2
root-directroy
data


FileSystem;   BlockWriter(FileSystem)
--Fat12
FileAllocator; BlockWriter(fileAllocator)
--Fat12

File  
-Regular
--Txt
--Binary
-Directroy





Block 


If:
IBlockDevice
    int read(int64_t offset, Block &);
    int write(const Block &, int64_t offset);
Imp:
BlockDevice<Block>

If:
IBlockChecker:
    bool check(const Block & block) //TODO: need to be abstracted

Imp:
Fat12FileSystemChecker
Fat12FileAllocatorChecker
Fat12RootDirectoryChecker
Fat12DataChecker


IF:
IBlockParser
    parse(const char * , len);
Imp:
Fat12BlockParser  --> Fat12FileSystem
Fat12AllocatorBlockParser  --> Fat12FileAllocator
FileMetaParser  --> File


    BlockDevice dev(fileName);
    FileSystemBlockChecker.check(dev.readBlock(0, 1));

    FileSystem fs = FileSystemBlockParser.parse(dev.readBlock(0, 1))
    fs.print(std::cout);
    FileAllocator faA = FileAllocatorBlockParser.parse(dev.readBlock(1, 9))
    fatA.print(std::cout);
    FileAllocator faB = FileAllocatorBlockParser.parse(dev.readBlock(9, 9))
    fatB.print(std::cout);
    RootDirectrory rd = FileAllocatorBlockReader.parse(dev.readBlock(18, 1));    
    rd.print(std::cout);
    





BlockWriter
    write(dev_id)
-- FileSystemBlockWriter
-- FileAllocatorBlockWriter
-- FileBlockWriter
-- RegularFileBlockWriter
