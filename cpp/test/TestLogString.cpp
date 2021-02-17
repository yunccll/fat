#include "gtest/gtest.h"
#include <memory>

#include "LogString.h"

using namespace helper;

class ObjectAsMember {
public:
	ObjectAsMember(){
		log.ctor();
	}
	~ObjectAsMember(){
		log.dtor();
	}

	ObjectAsMember & set(){
		log.method("set");
		return *this;
	}
	const ObjectAsMember & get() const {
		log.methodConst("get");
		return *this;
	}

	const LogString & getLogString() const{
		return log;
	}

private:
	mutable LogString log;
};
TEST(LogStringTest, asMember){
	ObjectAsMember obj;
	obj.set().get();

	{
		LogString expect;
		expect.ctor().method("set").methodConst("get");
		ASSERT_EQ(obj.getLogString(), expect);
	}
}


class ObjectAsParameter {
public:
	ObjectAsParameter(LogString & log)
	:log(log)
	{
		log.ctor();
	}
	~ObjectAsParameter()
	{
		log.dtor();
	}

	ObjectAsParameter & set(){
		log.method("set");
		return *this;
	}
	const ObjectAsParameter & get() const {
		log.methodConst("get");
		return *this;
	}

private:
	LogString & log;	
};

TEST(LogStringTest, asParameter){
	
	LogString logString;
	{
		ObjectAsParameter obj(logString);
		obj.set().get();
	}
	{
		LogString expect;
		expect.ctor().method("set").methodConst("get").dtor();
		ASSERT_EQ(expect, logString);
	}
}

class ObjectAsBase : public LogString {
public:
	ObjectAsBase()
	{
		ctor();
	}
	~ObjectAsBase()
	{
		dtor();
	}

	ObjectAsBase & set(){
		method("set");
		return *this;
	}
	const ObjectAsBase & get() const {
		methodConst("get");
		return *this;
	}
};

TEST(LogStringTest, asBase){

	ObjectAsBase *  obj = new ObjectAsBase();
	obj->set().get();
	{
		LogString expect ; 
		expect.ctor().method("set").methodConst("get");
		ASSERT_EQ(expect, obj->ref());
	}
}


//TODO: Global use method
//	#define LOGSTR	(helper::LogString::instance())
//	LOGSTR.reset().operator("functions").method("methods");
//	ASSERT_EQ(expect, LOGSTR);
//
//
//	#define EXPECT  (helper::LogString::instance())
//	 EXPECT.reset().operator("functions").method("methods");
//   ASSERT_EQ(EXPECT, obj->ref());
//
//TODO: easy way for usually using
//	logString = LogString::defaultObject(); ==> logString.reset().ctor().dtor();
//	logString = LogString::create(); ==> logString.reset().ctor();
