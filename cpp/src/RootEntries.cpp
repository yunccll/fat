#include "RootEntries.h"


namespace fat {


struct Entry {
public:
};

class RootEntries {
public:
    RootEntries(size_t capacity);
    ~RootEntries();

    bool add(const Entry * entry);
    bool remove(const char * path);
    Entry *  getEntry(const char * path) const;
    bool modify(const char * path, const Entry * newEntry);

    typedef Entry * EntryPointer;
private:
    std::unordered_map<std::string, EntryPointer> maps;
};


} //end of namespace fat
