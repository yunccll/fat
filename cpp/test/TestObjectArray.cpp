#include "gtest/gtest.h"

#include "helper/object/ObjectInterface.h"

#include <iostream>


using namespace std;
using namespace helper;
using namespace object;

TEST(ArrayTest, get) {
    Array * array = DefaultObjectFactory::instance().createArray();
    ASSERT_NE(nullptr, array);

    const Array::ValueType * value = array->get(0);
    ASSERT_EQ(nullptr, value);

    String * str = DefaultObjectFactory::instance().createString("hello");
    ASSERT_NE(nullptr, array);

    array->append(str);
    value = array->get(0);
    ASSERT_NE(nullptr, value);

    Object * obj = *value;
    ASSERT_NE(nullptr, obj);
    ASSERT_EQ(str, obj);

    String * strObj = (String *)obj;
    ASSERT_EQ("hello", strObj->getValue());

    delete array;
}

TEST(ArrayTest, appendNull) {
    Array * array = DefaultObjectFactory::instance().createArray();
    ASSERT_NE(nullptr, array);

    array->append(nullptr); 
    const Array::ValueType * value = array->get(0);
    ASSERT_NE(nullptr, value);
    ASSERT_EQ(nullptr, *value);

    delete array;
}

TEST(ArrayTest, traversal) {
    Array * array = DefaultObjectFactory::instance().createArray();
    ASSERT_NE(nullptr, array);
    
    String * str = DefaultObjectFactory::instance().createString("hello");
    ASSERT_NE(nullptr, array);
    array->append(str);
    array->append(nullptr); 

    ASSERT_EQ(2, array->size());

    auto it = array->iterator();
    while(it->hasNext()) {
        const Array::ValueType & value = it->next();
        if (nullptr != value) {
            cout << "type: " << value->getType() << endl;
        }
    }

    delete array;
}

TEST(ArrayTest, nextOutRange) {
    Array * array = DefaultObjectFactory::instance().createArray();
    ASSERT_NE(nullptr, array);

    auto it = array->iterator();
    const Array::ValueType & value = it->next();
    ASSERT_TRUE(it->__isOutOfRange(value));
}
