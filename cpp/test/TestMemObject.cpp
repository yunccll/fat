#include "gtest/gtest.h"

#include <functional>
#include "LogString.h"

namespace helper {
template<typename T>
class mem_ptr {
public:
	mem_ptr() : mem_ptr(nullptr, true){}

	// no explicit for auto-converting
	mem_ptr(std::nullptr_t null) : mem_ptr(null, true) {}

	template<typename V>
	mem_ptr(V * objPtr) :  mem_ptr(objPtr, true) {}

	//copy constructor
	mem_ptr(mem_ptr<T> && mem_obj) : mem_ptr(mem_obj.objPtr, true)
	{
		mem_obj.objPtr = nullptr;
	}

    ~mem_ptr(){
		release(objPtr);
	}

    inline T & operator *() {
#ifdef UTEST
		logString.method("operator*");
#endif
        return *objPtr;
    }
    inline T * operator ->(){
#ifdef UTEST
		logString.method("operator->");
#endif
        return objPtr;
    }
	inline T * get() const {
		return objPtr;
	}


	inline bool operator == (const mem_ptr<T> & mptr) const {
		return objPtr == mptr.objPtr;
	}
	inline bool operator !=(const mem_ptr<T> & mptr) const {
		return !(*this == mptr);
	}

	bool empty() const {
		return objPtr == nullptr;
	}


	mem_ptr<T> & operator =(mem_ptr<T> && mem_obj){
		if(!empty()){
			release(objPtr);
		}

		objPtr = mem_obj.objPtr;
		mem_obj.objPtr = nullptr;

		return *this;
	}

	T * pin() {
		canRelease = false;
		return objPtr;
	}


private:
	mem_ptr(T * objPtr, bool canRelease)
	: objPtr(objPtr)
	, canRelease(canRelease)
	{
	}
private:
	inline void release(T * objPtr){
		if(canRelease){
			delete objPtr;
		}
	}

private:
	mem_ptr(const mem_ptr<T> & mem_obj) = delete;
	mem_ptr<T> & operator =(const mem_ptr<T> & mem_obj) = delete;

private:
    T * objPtr;
	bool canRelease;

#ifdef UTEST
public:
	inline const helper::LogString & getLogString() const {
		return logString;
	}
private:
	helper::LogString logString;
#endif
};

template<typename T>
bool operator == (const void * ptr, const mem_ptr<T> & mptr){
	return ptr == mptr.get();
}
template<typename T>
bool operator != (const void * ptr, const mem_ptr<T> & mptr){
	return !(ptr == mptr);
}

} //end of namespace helper


//template<T>
//class mem_ptr_with_deleter : public mem_ptr<T> {
//public:
//	template<typename V>
//	mem_ptr_with_deleter(V* ptr, std::function<void (T*)> & deleter)
//	: mem_ptr(ptr)
//	, deleter(deleter)
//	{
//	}
//	~mem_ptr_with_deleter(){
//		deleter()
//	}
//private:
//	std::function<void (T*)> deleter;
//};

//TODO: customize the release function object from constructor
class Customize {
public:
	Customize(helper::LogString & logString)
	:logString(logString)
	{
		logString.ctor();
	}
	~Customize(){
		logString.dtor();
	}

	helper::LogString & getLogString() {
		return logString;
	}
private:
	helper::LogString & logString;
};
void ReleaseCustomize(void * obj){
	Customize * cus = static_cast<Customize*>(obj);
	delete cus;
}

// global func for release
// mem_func for release
// functor for release
TEST(mem_ptrTest, testCustomize){
//TODO:
//	helper::LogString logString;
//	{
//		helper::mem_ptr<Customize> pCus(new Customize(logString), &ReleaseCustomize);
//	}
//	ASSERT_EQ(helper::LogString().ctor().dtor(), logString);
	
}

class TestReleaseObject {
public:
	TestReleaseObject(helper::LogString & logString)
	:logString(logString)
	{
		logString.ctor();
	}
	~TestReleaseObject(){
		logString.dtor();
	}
private:
	helper::LogString & logString;
};


TestReleaseObject * createObject(helper::LogString & logString, bool failed){
	helper::mem_ptr<TestReleaseObject> pObjPtr(new TestReleaseObject(logString));
	if(failed){
		return nullptr;
	}
	return pObjPtr.pin();
}
// use scoped-object-ptr in error handler case
//OK -- give up the ownership from scoped-object-ptr if no-error
//OK -- free the dest-object if got error
TEST(mem_ptrTest, createObject){
	helper::LogString expect;
	{
		helper::LogString logString;
		TestReleaseObject * obj = createObject(logString, false);//create success
		ASSERT_NE(nullptr, obj);
		ASSERT_EQ(expect.reset().ctor(), logString);
		delete obj;
		ASSERT_EQ(expect.dtor(), logString);
	}
	{
		helper::LogString logString;
		TestReleaseObject * noReleaseObj = createObject(logString, true);// create failed
		ASSERT_EQ(nullptr, noReleaseObj);
		ASSERT_EQ( expect.reset().ctor().dtor(), logString);
	}
}


//release ptr if object is out of scope (****)
TEST(mem_ptrTest, testRelease){
	helper::LogString expect;
	helper::LogString logString;
	{
		helper::mem_ptr<TestReleaseObject> pObjPtr(new TestReleaseObject(logString));
	}
	ASSERT_EQ(expect.reset().ctor().dtor(), logString);
}


//   enable mem_ptr mem_obj = double *
//   enable mem_ptr mem_obj = std::move(mem_obj);
//  
//   disable:  
//		mem_ptr mem_obj;
//		mem_ptr mem_obj_new = mem_obj;
TEST(mem_ptrTest, testcopyConstructor){
	double * pRawDouble = new double (5.0);
	{
		helper::mem_ptr<double> pDouble = pRawDouble;
		ASSERT_TRUE(!pDouble.empty());
		{
			helper::mem_ptr<double> pOtherDouble = std::move(pDouble); // mem_ptr(mem_ptr &&)
			ASSERT_TRUE(pDouble.empty());
			ASSERT_TRUE(!pOtherDouble.empty());
		}

		//EXPECT(compile error) 
		{
			// helper::mem_ptr<double> pOtherDouble = pDouble; // mem_ptr( mem_ptr &&)
			
			//const helper::mem_ptr<double> pConstDouble;
			//EXPECT(compile error) 
			//helper::mem_ptr<double> pOtherDouble = pConstDouble;
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
TEST(mem_ptrTest, testCopyAssign){
	helper::mem_ptr<double> pDouble(new double(5.0));
	helper::mem_ptr<double> pOtherDouble(new double(6.0));
	{
		pOtherDouble = std::move(pDouble);
		ASSERT_TRUE(pDouble.empty());
		ASSERT_EQ(5.0, *pOtherDouble);
	}

	//EXPECT(compile error)
	{
		// pOtherDouble = pDouble;
	}
}


class One : public helper::LogString{
public:
    One()
    {
		ctor();
    }
    ~One(){
		dtor();
    }
    const std::string & getValue() const {
		methodConst("getValue");
        return value;
    }
    std::string & getValue() {
        method("getValue");
        return value;
    }
private:
	std::string value;
};
//OK -- container type(T) ptr
//OK -- <raw type> container dynamic raw object ptr
//OK	-- <*ptr>use operator * it as pointer
//OK	-- <ptr->> use operator -> it as pointer
//
//OK -- <const raw type> container dynamic const raw object ptr
//OK -- <custom type> container dynamic custom object ptr
//OK -- <const custom type> container dynamic const custom object ptr 
TEST(mem_ptrTest, testContainer){

	double expectDouble = 20.0f;
	helper::mem_ptr<double> pDouble(new double(expectDouble));
	ASSERT_TRUE(*pDouble == expectDouble);


	int expectInt = 10;
    helper::mem_ptr<int> pInt(new int(expectInt));
    ASSERT_TRUE(*pInt == expectInt);


	const int expectConstInt = 11;
    helper::mem_ptr<const int> pConstInt(new int(expectConstInt));
    ASSERT_TRUE(*pConstInt == expectConstInt);

	helper::LogString expect;
	{
		helper::mem_ptr<One> pOne(new One);
		pOne->getValue();
		ASSERT_EQ(expect.reset().ctor().method("getValue"), pOne->ref());

		pOne->getValue();
		ASSERT_EQ(expect.method("getValue"), pOne->ref());
	}
	{
		helper::mem_ptr<const One> pConstOne(new One);
		pConstOne->getValue();
		ASSERT_EQ(expect.reset().ctor().methodConst("getValue"), pConstOne->ref());
		//EXPECT(compile error) pConstOne->getValue() = "const two";
	}
}

#ifdef UTEST
//test for pOne internal LogString (operator * , operator->)
TEST(mem_ptrTest, internalTest){
	helper::LogString expect;
	{
		double expectDouble = 20.0f;
		helper::mem_ptr<double> pDouble(new double(expectDouble));
		ASSERT_TRUE(*pDouble == expectDouble);
		ASSERT_EQ(expect.reset().method("operator*"), pDouble.getLogString());
	}

	{
		helper::mem_ptr<One> pOne(new One);
		pOne->getValue();
		ASSERT_EQ(expect.reset().method("operator->"), pOne.getLogString());
	}
}
#endif 

//OK -- compare NullObject with nullptr ( nullObject == nullptr | nullptr == NullObject)
//OK --	unequal to nullptr
//
//OK -- compare NullObject with NULL
//OK -- unequal to NULL
//
//OK --	compare two NullObject of ptr
//OK --	unequal between NullObject
//OK -- NullObject inited with nullptr Or NULL
TEST(mem_ptrTest, testNullObject) {

	helper::mem_ptr<double> pNullPtr;
	ASSERT_TRUE(pNullPtr == nullptr);
	ASSERT_TRUE(nullptr == pNullPtr);
	ASSERT_FALSE(pNullPtr != nullptr);
	ASSERT_FALSE(nullptr != pNullPtr);

	ASSERT_TRUE(nullptr == NULL);
	ASSERT_TRUE(pNullPtr == NULL);

	helper::mem_ptr<double> pNullPtr1;
	ASSERT_TRUE(pNullPtr == pNullPtr1);
	ASSERT_FALSE(pNullPtr != pNullPtr1);

	helper::mem_ptr<double> pNullPtrInitWith(nullptr);
	ASSERT_TRUE(pNullPtr == pNullPtrInitWith);
}

//TODO: big case --> const object pointer
//TODO: scoped-object-array-ptr 
//TODO: change a good name for helper::mem_ptr

