#include "gtest/gtest.h"

namespace helper {
template<typename T>
class mem_ptr {
public:
    template<typename V>
    mem_ptr(V * objPtr)
    :objPtr(objPtr)
    {
    }
    ~mem_ptr(){
    }

    T & operator *() {
        std::cout << "operator *" << std::endl;
        return *objPtr;
    }
    T * operator ->(){
        std::cout << "operator ->" << std::endl;
        return objPtr;
    }

//    const T & operator *() const {
//        std::cout << "const operator *" << std::endl;
//        return *objPtr;
//    }
private:
    T * objPtr;
};
}


class One {
public:
    One(const std::string & value)
    :value(value)
    {
        std::cout << "constuctor One:[" << value << "]" << std::endl;
    }
    ~One(){
        std::cout << "destructor One:[" << value << "]" << std::endl;
    }
    const std::string & getValue() const {
        std::cout << "const One::getValue const: ["  << value << "]" << std::endl;
        return value;
    }
    std::string & getValue() {
        std::cout << "One::getValue:[" << value << "]" << std::endl;
        return value;
    }
private:
    std::string value;
};


//TODO:  scoped-object-ptr
// container type(T) ptr
//OK -- container dynamic raw object ptr
//OK -- container dynamic const raw object ptr
//OK -- container dynamic custom object ptr
//OK -- container dynamic const custom object ptr 
//OK -- use it as pointer
//
// use as non-const object pointer
// use as const object pointer
//
//  Null object 
// release ptr if object is out of scope (****)
//
// give up the ownership when needed
//
TEST(mem_ptrTest, container){

    helper::mem_ptr<double> pDouble(new double(20.0));
    ASSERT_TRUE(*pDouble == 20.0);
    helper::mem_ptr<int> pInt(new int(10));
    ASSERT_TRUE(*pInt == 10);

    helper::mem_ptr<const int> pConstInt(new int(11));
    ASSERT_TRUE(*pConstInt == 11);

    helper::mem_ptr<One> pOne(new One("one"));
    ASSERT_EQ(std::string("one"), pOne->getValue());
    pOne->getValue() = "two";
    ASSERT_EQ(std::string("two"), pOne->getValue());

    helper::mem_ptr<const One> pConstOne(new One("const one"));
    ASSERT_EQ(std::string("const one"), pConstOne->getValue());
    // pConstOne->getValue() = "const two"; compiled error
}



//TODO: scoped-object-array-ptr 
//  container dynamic array object ptr
//
