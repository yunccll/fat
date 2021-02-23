#include "gtest/gtest.h"

#include "helper/object/ObjectInterface.h"
#include "helper/ScopedMemPtr.h"

//TODO: Object *
//  Integer 
//  Double 
//  String 
//  BYTE
//  ARRAY 
//  HASH
//  uint16, int16, uint32, int32, uint64, int64 

using helper::object::DefaultObjectFactory;
using helper::object::Object;
using helper::object::Integer;

TEST(ObjectIntegerTest, testCreate){
    int expect = 1000;

    auto pObj = helper::ScopedMemPtr<Object>(DefaultObjectFactory::instance().createInteger(expect));
    ASSERT_TRUE(pObj->isInteger());

    auto psub = helper::ScopedMemPtr<Integer>(DefaultObjectFactory::instance().createInteger(expect));
    ASSERT_EQ(expect, psub->get());
}


using helper::object::Double;
TEST(ObjectDoubleTest, testCreate){
    double expect = 1000.0;

    auto pObj = helper::ScopedMemPtr<Object>(DefaultObjectFactory::instance().createDouble(expect));
    ASSERT_TRUE(pObj->isDouble());

    auto psub = helper::ScopedMemPtr<Double>(DefaultObjectFactory::instance().createDouble(expect));
    ASSERT_EQ(expect, psub->get());
}

using helper::object::Byte;
TEST(ObjectByteTest, testCreate){
    char expect = '1';

    auto pObj = helper::ScopedMemPtr<Object>(DefaultObjectFactory::instance().createByte(expect));
    ASSERT_TRUE(pObj->isByte());

    auto psub = helper::ScopedMemPtr<Byte>(DefaultObjectFactory::instance().createByte(expect));
    ASSERT_EQ(expect, psub->get());
}






TEST(ObjectStringTest, testCreate){
    
    ASSERT_EQ(1, 1);
}
TEST(ObjectArrayTest, testCreate){
    
    ASSERT_EQ(1, 1);
}
//TODO:Hash
// >Create :
//      > new 
//      > factory 
// >insert 
// >remove
// >iterate
// >destroy
TEST(ObjectHashTest, testCreate){
    
    ASSERT_EQ(1, 1);
}

