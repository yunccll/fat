#ifndef  HELPER_UNITTEST_TESTOBJECTS_H
#define  HELPER_UNITTEST_TESTOBJECTS_H

#include "helper/unittest/Tracer.h"

namespace helper {
namespace unittest {

class ObjectAsRef : public ITracerAsRef{
public:
	ObjectAsRef(helper::unittest::Tracer & tracer)
	:ITracerAsRef(tracer)
	{
        getTracer().ctor();
	}
	~ObjectAsRef() override {
        getTracer().dtor();
	}

	inline ObjectAsRef & set(){
		getTracer().method("set");
		return *this;
	}
	inline const ObjectAsRef & get() const {
		getTracer().methodConst("get");
		return *this;
	}
	inline ObjectAsRef & get() {
		getTracer().method("get");
		return *this;
	}
};
class ObjectAsMember : public ITracerAsMembe{
public:
	ObjectAsMember(){
		getTracer().ctor();
	}
	~ObjectAsMember() override {
		getTracer().dtor();
	}

	inline ObjectAsMember & set(){
		getTracer().method("set");
		return *this;
	}
	inline const ObjectAsMember & get() const {
		getTracer().methodConst("get");
		return *this;
	}
};

class ObjectAsBase : public Tracer {
public:
	ObjectAsBase()
	{
		ctor();
	}
	~ObjectAsBase() override {
		dtor();
	}

	inline ObjectAsBase & set(){
		method("set");
		return *this;
	}
	inline const ObjectAsBase & get() const {
		methodConst("get");
		return *this;
	}
};

} //end of namespace unittest
} //end of namespace helper

#endif   /* HELPER_UNITTEST_TESTOBJECTS_H */
