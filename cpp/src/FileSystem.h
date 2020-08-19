#pragma once

#include <memory>

#include "Status.h"

namespace fat {

class FsInfo;


class FileSystem {

public:
    FileSystem(const std::string & name);
    ~FileSystem();
    static std::shared_ptr<FileSystem> getDefaultFileSystem();

    Status isExist(const std::string & path, bool & exist);
    Status mount();
    Status unmount();
    Status stat(FsInfo & fsInfo );

private:
    Status loadMeta();

private:
    std::string name;

    //FileDevice device;


    static std::shared_ptr<FileSystem> __inst;
};

} //end of namespace fat
