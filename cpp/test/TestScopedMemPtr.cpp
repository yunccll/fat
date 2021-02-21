#include "gtest/gtest.h"
#include "helper/unittest/TestObjects.h"
#include "helper/ScopedMemPtr.h"



using helper::unittest::ObjectAsRef;
using helper::unittest::Tracer;

TEST(ScopedMemPtrTest, testContainerRaw){
	double expectDouble = 20.0f;
	auto pDouble = helper::make_scoped<double>(expectDouble);
	ASSERT_TRUE(*pDouble == expectDouble);

	int expectInt = 10;
    auto pInt = helper::make_scoped<int>(expectInt);
    ASSERT_TRUE(*pInt == expectInt);

	const int expectConstInt = 11;
    auto pConstInt = helper::make_scoped<const int>(expectConstInt);
    ASSERT_TRUE(*pConstInt == expectConstInt);
}


//release ptr if object is out of scope (****)
TEST(ScopedMemPtrTest, testReleaseObject){
    EXPECT.reset().ctor().method("set").method("get").dtor();
    {
        auto objPtr = helper::make_scoped<ObjectAsRef> (TRACER.reset());
        objPtr->set();
        objPtr->get();
        // ObjectAsRef is deleted when ScopedMemPtr is destructed
    }
    ASSERT_EQ(EXPECT, TRACER);
}

TEST(ScopedMemPtrTest, testReleaseConstObject){
    EXPECT.reset().ctor().methodConst("get").dtor();
    {
        auto objPtr = helper::make_scoped<const ObjectAsRef>(TRACER.reset());
        objPtr->get();
    }
    ASSERT_EQ(EXPECT, TRACER);
}


TEST(ScopedMemPtrTest, testNotReleaseObject){

    ObjectAsRef * obj = new ObjectAsRef(TRACER.reset()); 
    EXPECT.reset().ctor();
    {
        helper::ScopedMemPtr<ObjectAsRef> objPtr(obj, false);
    }
    ASSERT_EQ(EXPECT, TRACER);

    //clear resource by use
    delete obj;
    ASSERT_EQ(EXPECT.dtor(), TRACER);
}


static ObjectAsRef * createObject(Tracer & tracer, bool failed){
    auto pObjPtr = helper::make_scoped<ObjectAsRef>(tracer);
	if(failed){
		return nullptr;
	}
	return pObjPtr.pin();
}
TEST(ScopedMemPtrTest, testCreateObjectSuccess){
    ObjectAsRef * obj = createObject(TRACER.reset(), false);
    ASSERT_NE(nullptr, obj);
    ASSERT_EQ(EXPECT.reset().ctor(), TRACER);

    delete obj;
    ASSERT_EQ(EXPECT.dtor(), TRACER);
}
TEST(ScopedMemPtrTest, testCreateObjectFailed){
    ObjectAsRef * obj = createObject(TRACER.reset(), true);
    ASSERT_EQ(nullptr, obj);

    ASSERT_EQ(EXPECT.reset().ctor().dtor(), TRACER);
}

//OK -- compare NullObject with nullptr ( nullObject == nullptr | nullptr == NullObject)
//OK --	unequal to nullptr
//
//OK -- compare NullObject with NULL
//OK -- unequal to NULL
//
//OK --	compare two NullObject of ptr
//OK --	unequal between NullObject
//OK -- NullObject inited with nullptr Or NULL
TEST(ScopedMemPtrTest, testNullObject) {
	helper::ScopedMemPtr<double> pNullPtr;
	ASSERT_TRUE(pNullPtr == nullptr);
	ASSERT_TRUE(nullptr == pNullPtr);
	ASSERT_FALSE(pNullPtr != nullptr);
	ASSERT_FALSE(nullptr != pNullPtr);

	ASSERT_TRUE(nullptr == (std::nullptr_t)NULL);
	ASSERT_TRUE(pNullPtr == NULL);

	helper::ScopedMemPtr<double> pNullPtr1;
	ASSERT_TRUE(pNullPtr == pNullPtr1);
	ASSERT_FALSE(pNullPtr != pNullPtr1);

	helper::ScopedMemPtr<double> pNullPtrInitWith(nullptr);
	ASSERT_TRUE(pNullPtr == pNullPtrInitWith);
}


//   enable ScopedMemPtr mem_obj = double *
//   enable ScopedMemPtr mem_obj = std::move(mem_obj);
//  
//   disable:  
//		ScopedMemPtr mem_obj;
//		ScopedMemPtr mem_obj_new = mem_obj;
TEST(ScopedMemPtrTest, testcopyConstructor){
	double * pRawDouble = new double (5.0);
	{
        // pRawDouble -> const ScopedMemPtr && ->  ScopedMemPtr(const ScopedMemPtr &)
		helper::ScopedMemPtr<double> pDouble = pRawDouble;//  
		ASSERT_TRUE(!pDouble.empty());
		{
			helper::ScopedMemPtr<double> pOtherDouble = std::move(pDouble);
			ASSERT_TRUE(pDouble.empty());
			ASSERT_TRUE(!pOtherDouble.empty());
		}

		//EXPECT(compile error) 
		{
			//helper::ScopedMemPtr<double> pOtherDouble = pDouble; // copy assign
			
			//const helper::ScopedMemPtr<double> pConstDouble;
			//EXPECT(compile error) 
			//helper::ScopedMemPtr<double> pOtherDouble = pConstDouble;
		}
	}
}


// enable: 
//	mem_obj = std::move(mem_obj_other)
//	mem_obj --> other_obj
//  free mem_obj --> obj
//  mem_obj_other --> nullptr
//
// disable: 
//	mem_obj = mem_obj_other;
//	mem_obj = const_mem_obj_other;
TEST(ScopedMemPtrTest, testMoveAssign){
	auto pDouble = helper::make_scoped<double>(5.0);
	auto pOtherDouble = helper::make_scoped<double>(6.0);
	{
		pOtherDouble = std::move(pDouble);
		ASSERT_TRUE(pDouble.empty());
		ASSERT_EQ(5.0, *pOtherDouble);
	}

	//EXPECT(compile error)
	{
		//pOtherDouble = pDouble;
	}
}

//refactor ScopedMemPtr constructor to easy use
#include <sstream>
struct Args{
    Args(): i(0), j(0), k(0) {}
    Args(const int & i) :i(i), j(0), k(0) {}
    Args(int && i) : i(std::move(i)), j(0), k(0) {}

    Args(int & i, const int & j) : i(i), j(j), k(0) {}
    Args(int & i, const int & j, int & k) : i(i), j(j), k(k) {}

    std::string toString() const {
        std::ostringstream oss;
        oss << "i:" << i  << " j:" << j << " k:" << k;
        return oss.str();
    }
private:
    int i;
    int j;
    int k;
};
TEST(ScopedMemPtrTest, testNew){
    {
        auto obj = helper::make_scoped<Args>();
        ASSERT_FALSE(obj.empty());
    }
    {
        auto obj = helper::make_scoped<Args>(1);
        ASSERT_FALSE(obj.empty());
    }
    {
        int i = 0;
        auto obj = helper::make_scoped<Args>(i);
        ASSERT_FALSE(obj.empty());
    }
    {
        int i = 0; int j = 0;
        auto obj = helper::make_scoped<Args>(i, j);
        ASSERT_FALSE(obj.empty());
    }
}

//TODO: DefaultReleaser maybe an singlton,
//  all Releaser maybe singleton, don't need to free 
//TODO: big case --> const object pointer
//TODO: scoped-object-array-ptr 
