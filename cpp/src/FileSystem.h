#pragma once

#include <memory>

#include "Status.h"

namespace fat {

class FileSystem {

public:
    FileSystem(const std::string & name);
    ~FileSystem();
    static std::shared_ptr<FileSystem> getDefaultFileSystem();

    Status isExist(const std::string & path, bool & exist);
    Status mount();
    Status unmount();
    Status stat();

private:
    std::string name;
    static std::shared_ptr<FileSystem> __inst;
};

} //end of namespace fat
