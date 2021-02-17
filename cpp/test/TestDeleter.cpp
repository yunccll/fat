#include "gtest/gtest.h"
#include "LogString.h"
#include <functional>

template<typename T>
struct Deleter {
	Deleter(){}
	explicit Deleter(std::function<void (T*)> deleter) : deleter(deleter){}
	void operator()(T * obj){
		if(deleter)
			deleter(obj);
		else{
			delete obj;
		}
	}
private:
	std::function<void (T*)> deleter;
};
void delInt(int * ptr){
	std::cout << "delInt:" << *ptr << std::endl;
	delete ptr;
}
struct MemFuncDeleter{
	void release(int * ptr){
		std::cout << "release from mem_func:" << *ptr << std::endl;
		delete ptr;
	}
};

TEST(DeleterTest, test){
	{// default deleter
		Deleter<int> deleter;
		deleter(new int(5));
	}

	{ //global_func
		Deleter<int> deleter(&delInt);
		deleter(new int(10));
	}

	{ // mem_func
		MemFuncDeleter mem;
		Deleter<int> deleter(std::bind(&MemFuncDeleter::release, &mem, std::placeholders::_1));
		deleter(new int(40));
	}

	{ //lambda func
		Deleter<int> deleter([](int * ptr) { 
			std::cout << "lambda:" << *ptr << std::endl;
			delete ptr;
		});
		deleter(new int(30));
	}

	{ // functor
		std::function<void (int*)> func = [](int * ptr) { 
			std::cout << "lambda:" << *ptr << std::endl;
			delete ptr;
		};
		Deleter<int> deleter(func);
		deleter(new int(20));
	}
}

/*  
class ILogStringAsRef{
public:
	ILogStringAsRef(helper::LogString & logString)
	:logString(logString)
	{
		logString.ctor();
	}
	virtual ~ILogStringAsRef(){
		logString.dtor();
	}

	virtual helper::LogString & getLogString() {
		return logString;
	}
private:
	helper::LogString & logString;
};
//TODO: ILogStringAsMember
//TODO: ILogStringAsBase
class Object : public ILogStringAsRef{
public:
	Object(helper::LogString & logString)
	:ILogStringAsRef(logString)
	{
	}
	~Object()override{
	}
};

template<typename T>
void global_deleter_ILogStringAsRef(T * obj){
	auto * objAsRef = static_cast<ILogStringAsRef*>(obj);
	if(objAsRef){
		objAsRef->getLogString().operation("global_deleter");
		delete objAsRef;
	}
}



// global func for release
TEST(DeleterTest, testGlobalFunction){
	{
		helper::LogString logString;
		auto * obj = new Object(logString);
		delete obj;
		ASSERT_EQ(helper::LogString().ctor().dtor(), logString);
	}
	{
		helper::LogString logString;
		auto obj = new Object(logString);
		global_deleter_ILogStringAsRef(obj);
		ASSERT_EQ(helper::LogString().ctor().operation("global_deleter").dtor(), logString);
	}
	{
		helper::LogString logString;
		auto obj = new Object(logString);
		Deleter<Object> deleter(&global_deleter_ILogStringAsRef<Object>);
		deleter(obj);
		ASSERT_EQ(helper::LogString().ctor().operation("global_deleter").dtor(), logString);
	}
}


// mem_func for release

// functor for release
// lambda for release
//
//
// deleter type:  global_deleter;  mem_deleter; functor deleter
template<typename T>
struct ObjectDeleter{
	void operator()(T * obj){
		release(obj);
	}
protected:
	virtual void release(T * obj) {
		delete obj;
	}
};


TEST(DeleterTest, testFunctor){
	{
		helper::LogString logString;
		auto obj = new Object(logString);
		ObjectDeleter<Object> deleter; deleter(obj);
		ASSERT_EQ(helper::LogString().ctor().dtor(), logString);
	}
}

class MyObject{};
struct MyObjectDeleter : public ObjectDeleter<MyObject>{
	virtual void release(MyObject * obj){
		std::cout << "release MyObject" << std::endl;
		delete obj;
	}
};
TEST(DeleterTest, use){
	auto * obj = new MyObject;
	MyObjectDeleter deleter; deleter(obj);
}*/
