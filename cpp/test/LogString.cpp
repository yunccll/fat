#include "LogString.h"

namespace helper{

const std::string LogString::SEP_METHOD = ":";
const std::string LogString::SUFFIX_CONST_METHOD = "_const";

bool operator == (const std::string & str, const LogString & logString){
	return logString == str;
}
bool operator != (const std::string & str, const LogString & logString){
	return !(str == logString);
}

}// end of namespace helper
