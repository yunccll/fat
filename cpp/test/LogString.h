
#ifndef  HELPER_LOGSTRING_H
#define  HELPER_LOGSTRING_H

#include <string>

namespace helper{

class LogString {
public:
	static const std::string SEP_METHOD;
	static const std::string SUFFIX_CONST_METHOD;

	LogString() {}
	LogString(const std::string & initString) : str(initString) {}

	virtual ~LogString(){}

	const LogString & reset() const {
		str.clear();
		return *this;
	}


public:
	LogString * ptr(){
		return this;
	}
	const LogString * ptr() const {
		return this;
	}

	LogString & ref(){
		return *this;
	}
	const LogString & ref() const {
		return *this;
	}

public:
	inline const LogString & ctor()const{
		return ctor("ctor");
	}
	inline const LogString &  ctor(const std::string & ctorName) const{
		return sep().append(ctorName);
	}

	inline const LogString &  dtor() const{
		return dtor("dtor");
	}
	inline const LogString & dtor(const std::string & dtorName) const{
		return sep().append(dtorName);
	}

	inline const LogString & method(const std::string & nameMethod) const{
		return sep().append(nameMethod);
	}
	inline const LogString & methodConst(const std::string & nameMethod) const{
		return method(nameMethod).withConst();
	}

	inline const LogString & operation(const std::string & nameOperation) const{
		return sep().append(nameOperation);
	}

	inline const LogString & sep() const {
		return append(SEP_METHOD);
	}
	inline const LogString & withConst() const{
		return append(SUFFIX_CONST_METHOD);
	}

private:
	const LogString & append(const std::string & content) const{
		str += content;
		return *this;
	}
	const LogString & append(const char * content, size_t n) const{
		str.append(content, n);
		return *this;
	}


public:
	bool operator == (const LogString & logString) const {
		return str == logString.str;
	}
	bool operator != (const LogString & logString) const {
		return !(*this == logString);
	}
	bool operator == (const std::string & str) const {
		return this->str == str;
	}
	bool operator != (const std::string & str) const {
		return !(*this == str);
	}

	const std::string & toString() const {
		return str;
	}

private:
	mutable std::string str;
};


bool operator == (const std::string & str, const LogString & logString);
bool operator != (const std::string & str, const LogString & logString);


}// end of namespace helper

#endif   /* HELPER_LOGSTRING_H */
