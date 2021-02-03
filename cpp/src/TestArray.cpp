#include "gtest/gtest.h"
#include <vector>
#include <algorithm>
class Object {
public:
    bool isNull(){ return true; }
};

class Array {
public:

    typedef Object * ValueType;

    virtual ~Array() {}

    virtual Array::ValueType & get(const int index) = 0; // { return Array::NullObject;}
    virtual const Array::ValueType & get(const int index) const = 0; 
    

    virtual bool isNull(ValueType & v) const {
        return &v == &NullObject;
    }
    static ValueType NullObject;
};
Array::ValueType Array::NullObject;


TEST(ArrayTest, nullObjectEqual){
    static Array::ValueType a;
    static Array::ValueType b;
    ASSERT_FALSE(&a == &b);
}


class StandardArray : public Array {
public:
    StandardArray() {
        Array::ValueType obj = new Object();
        values.push_back(obj);
    }
    ~StandardArray() override {
        std::for_each(values.begin(), values.end(), 
            [](Array::ValueType & val){
                delete val;
        });
    };
    Array::ValueType & get(const int index) override {
        if(index >= 0 && (size_t)index < values.size()){
            return values[index];
        }
        return Array::NullObject;
    }

    const Array::ValueType & get(const int index) const override {
        if(index >= 0 && (size_t)index < values.size()){
            return values[index];
        }
        return Array::NullObject;
    }

private:
    std::vector<Array::ValueType> values;
};


TEST(ArrayTest, use_ArrayNullObject){
    Array * array = new StandardArray();
    auto & refv = array->get(0);
    ASSERT_FALSE(array->isNull(refv));

    ASSERT_TRUE(array->isNull(array->get(1)));
    delete array;
}

class NewArray : public Array {
public:
    NewArray() {
        Array::ValueType obj = new Object();
        values.push_back(obj);
    }
    ~NewArray() override {
        std::for_each(values.begin(), values.end(), 
            [](Array::ValueType & val){
                delete val;
            }
        );
    };
    Array::ValueType & get(const int index) override {
        if(index >= 0 && (size_t)index < values.size()){
            return values[index];
        }
        return NewArray::NullObject;
    }

    const Array::ValueType & get(const int index) const override {
        if(index >= 0 && (size_t)index < values.size()){
            return values[index];
        }
        return NewArray::NullObject;
    }
    
    bool isNull(ValueType & v) const override {
        return &v == &NewArray::NullObject;
    }
    static Array::ValueType NullObject;

private:
    std::vector<Array::ValueType> values;
};
Array::ValueType NewArray::NullObject;

TEST(ArrayTest, use_NewArrayNullObject){
    Array * array = new NewArray();
    auto & refv = array->get(0);
    ASSERT_FALSE(array->isNull(refv));

    ASSERT_TRUE(array->isNull(array->get(1)));
    delete array;
}
