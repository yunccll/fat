
#ifndef  GTPCPP_DATA_STANDARDOBJECT_H
#define  GTPCPP_DATA_STANDARDOBJECT_H

#include <string>
#include <vector>
#include <unordered_map>

#include "helper/object/ObjectInterface.h"

namespace helper {
namespace object {

class StandardString : public String{
public:
    StandardString();
    explicit StandardString(const std::string & str);
    ~StandardString() override;

    void setValue(const std::string & value) override;
    const std::string & getValue() const override;
    int size() const override;

    void append(const std::string & str) override;
    void append(char ch) override;

    std::string toString() const override;
    int compare(const Object * obj) const override;
    Object * copy() const override;
private:
    std::string value_;
};

class StandardArray : public Array{
public:
    StandardArray();
    ~StandardArray() override;

    
    int size() const override;
    void append(const Array::ValueType & o) override;
    const Array::ValueType * get(const int index) const override;

    std::shared_ptr<Iterator<Array::ValueType>> iterator() const override;

    Array::ValueType remove(const int index) override;
    Array::ValueType set(const int index, const ValueType & value) override;

    std::string toString() const override;
    int compare(const Object * obj) const override;
    Object * copy() const override;

protected:
    friend class StandardArrayIterator;  
    void freeHolderResource(); 
private:
    std::vector<Array::ValueType> array;
};

class StandardArrayIterator : public Iterator<Array::ValueType> {
public:
    explicit StandardArrayIterator(Array & array);
    explicit StandardArrayIterator(const Array & array);
    ~StandardArrayIterator() override;

    bool hasNext() const override;
    const ElementType & next() override;

private:
    const Array & arrayRef;
    int index;
};


class StandardHash : public Hash {
public:
    StandardHash();
    ~StandardHash() override;

    int size() const override;
    const Hash::Pair::second_type * get(const std::string  & key) const override;

    bool containsKey(const std::string & key) const override;
    int insert(const std::string & key, const std::string & value) override;
    int insert(const std::string & key, const Hash::Pair::second_type &o) override;

    std::shared_ptr<Iterator<Hash::Pair>> iterator() const override;

    Hash::Pair::second_type remove(const Hash::Pair::first_type & key) override;
    Hash::Pair::second_type set(const Hash::Pair::first_type & key, Hash::Pair::second_type & value) override;

    std::string toString() const override;
    int compare(const Object * obj) const override;
    Object * copy() const override;

protected:
    friend class StandardHashIterator;  
    void freeHolderResource(); 
private:
    typedef std::unordered_map<std::string, Object *> HashImp;
    HashImp hash;
};

class StandardHashIterator : public Iterator<Hash::Pair> {
public:
    explicit StandardHashIterator(Hash & hash);
    explicit StandardHashIterator(const Hash & hash);
    ~StandardHashIterator() override;

    bool hasNext() const override;
    const ElementType & next() override;

private:
    const Hash & hashRef;
    StandardHash::HashImp::iterator iter;
    StandardHash::HashImp::iterator end;
};

class StandardByte : public Byte {
public:
    explicit StandardByte(const char value);
    ~StandardByte() override;

    const char set(const char value) override;
    const char get() const override;

    std::string toString() const override;
    int compare(const Object * obj) const override;
    Object * copy() const override;
private:
    char value_;
};

class StandardInteger : public Integer {
public:
    explicit StandardInteger(const long long value);
    ~StandardInteger() override;

    const long long set(const long long value) override;
    const long long get() const override;

    std::string toString() const override;
    int compare(const Object * obj) const override;
    Object * copy() const override;
private:
    long long value_;
};
class StandardDouble : public Double {
public:
    explicit StandardDouble(const double value);
    ~StandardDouble() override;

    const double set(const double value) override;
    const double get() const override;

    std::string toString() const override;
    int compare(const Object * obj) const override;
    Object * copy() const override;
private:
    double value_;
};

} // end of namespace object
} // end of namespace helper

#endif   /* GTPCPP_DATA_STANDARDOBJECT_H */
