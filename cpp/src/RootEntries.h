#pragma once


#include <string>
#include <memory>
#include <unordered_map>
#include <ostream>



namespace fat {


struct Entry;
struct DirItem;

class RootEntries {
public:
    typedef std::shared_ptr<Entry> EntryPointer;

    bool add(const char* path, const EntryPointer entry);
    EntryPointer remove(const char * path);
    EntryPointer  getEntry(const char * path) const;
    EntryPointer modify(const char * path, const EntryPointer newEntry);

    bool add(const std::string & path, const EntryPointer entry);
    EntryPointer remove(const std::string & path);
    EntryPointer  getEntry(const std::string & path) const;
    EntryPointer modify(const std::string & path, const EntryPointer newEntry);

    std::ostream & operator<<(std::ostream & os) const;
private:
    std::unordered_map<std::string, EntryPointer> maps;
};

std::ostream & operator<< (std::ostream & os, const RootEntries & );


class RootEntryArray {
public:
    RootEntryArray(const char * data, size_t len);
    RootEntryArray(char * data, size_t len);
    RootEntryArray(uint8_t * data, size_t len);
    ~RootEntryArray();

    size_t getEntryCount() const;

    typedef struct DirItem value_type;
    typedef value_type& reference;
    typedef value_type* pointer;

    reference operator[](size_t index) const ;
    //read/write
    reference operator[](size_t index);

    class iterator {
    public:
        iterator(uint8_t * data, size_t index, size_t step)
        :data(data)
        ,index(index)
        ,step(step)
        {
        }
        ~iterator(){}


        typedef iterator self_type;
        typedef DirItem value_type;
        typedef value_type& reference;
        typedef value_type* pointer;

        //++iterator
        iterator & operator++(){
            index += step;
            return *this;
        }
        //iterator++
        iterator operator++(int){
            auto old = *this;
            index += step;
            return old;
        }
        //--iterator
        iterator & operator--(){
            index -= step;
            return *this;
        }
        //iterator--
        iterator operator--(int){
            auto old = *this;
            index -= step;
            return old;
        }
        reference operator *(){
            return *(reinterpret_cast<pointer>(data + index));
        }
        pointer operator->(){
            return reinterpret_cast<pointer>(data+index);
        }
        bool operator ==(const iterator & iter){
            return data == iter.data && index == iter.index;
        }
        bool operator !=(const iterator & iter){
            return ! (*this==iter);
        }

    private:
        uint8_t * data;
        size_t index;
        size_t step;
    };
    //range read 
    iterator begin() ;
    iterator end();

private:
    uint8_t * data;
    size_t len;
};


} //end of namespace fat
