
#ifndef  HELPER_UNITTEST_LOGSTRING_H
#define  HELPER_UNITTEST_LOGSTRING_H

#include <string>

namespace helper{
namespace unittest{

class Tracer {
public:
	static const std::string SEP_METHOD;
	static const std::string SUFFIX_CONST_METHOD;

	Tracer() {}
	explicit Tracer(const std::string & initString) : str(initString) {}

	virtual ~Tracer(){}

	inline const Tracer & reset() const {
		str.clear();
		return *this;
	}
	inline Tracer & reset(){
		str.clear();
		return *this;
	}


public:
	Tracer * ptr(){
		return this;
	}
	const Tracer * ptr() const {
		return this;
	}

	Tracer & ref(){
		return *this;
	}
	const Tracer & ref() const {
		return *this;
	}

public:
	inline const Tracer & ctor()const{
		return ctor("ctor");
	}
	inline const Tracer &  ctor(const std::string & ctorName) const{
		return sep().append(ctorName);
	}

	inline const Tracer &  dtor() const{
		return dtor("dtor");
	}
	inline const Tracer & dtor(const std::string & dtorName) const{
		return sep().append(dtorName);
	}

	inline const Tracer & method(const std::string & nameMethod) const{
		return sep().append(nameMethod);
	}
	inline const Tracer & methodConst(const std::string & nameMethod) const{
		return method(nameMethod).withConst();
	}

	inline const Tracer & operation(const std::string & nameOperation) const{
		return sep().append(nameOperation);
	}

	inline const Tracer & sep() const {
		return append(SEP_METHOD);
	}
	inline const Tracer & withConst() const{
		return append(SUFFIX_CONST_METHOD);
	}

private:
	const Tracer & append(const std::string & content) const{
		str += content;
		return *this;
	}
	const Tracer & append(const char * content, size_t n) const{
		str.append(content, n);
		return *this;
	}


public:
	bool operator == (const Tracer & tracer) const {
		return str == tracer.str;
	}
	bool operator != (const Tracer & tracer) const {
		return !(*this == tracer);
	}
	bool operator == (const std::string & str) const {
		return this->str == str;
	}
	bool operator != (const std::string & str) const {
		return !(*this == str);
	}

	const std::string toString() const {
		return str;
	}

private:
	mutable std::string str;
};

bool operator == (const std::string & str, const Tracer & tracer);
bool operator != (const std::string & str, const Tracer & tracer);



class TracerFactory {
public:
    static Tracer & tracer(){
        return TracerFactory::__tracer;
    }

    static Tracer & expect(){
        return TracerFactory::__expect;
    }
private:
    static Tracer __tracer;
    static Tracer __expect;
};

#define TRACER       (helper::unittest::TracerFactory::tracer())
#define EXPECT      (helper::unittest::TracerFactory::expect())

class ITracerAsRef{
public:
	ITracerAsRef(helper::unittest::Tracer & tracer)
	:tracer(tracer)
	{
	}
	virtual ~ITracerAsRef(){
	}

	virtual helper::unittest::Tracer & getTracer() {
		return tracer;
	}
	virtual const helper::unittest::Tracer & getTracer() const {
		return tracer;
	}
private:
	helper::unittest::Tracer & tracer;
};

class ITracerAsMembe {
public:
    virtual ~ITracerAsMembe(){}

    virtual helper::unittest::Tracer & getTracer() {
        return tracer;
    }
    virtual const helper::unittest::Tracer & getTracer() const {
        return tracer;
    }
private:
    helper::unittest::Tracer tracer; 
};

}// end of namespace unittest
}// end of namespace helper

#endif   /* HELPER_UNITTEST_LOGSTRING_H */
