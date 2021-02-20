#include "Tracer.h" 


namespace helper{
namespace unittest{

const std::string Tracer::SEP_METHOD = ":";
const std::string Tracer::SUFFIX_CONST_METHOD = "_const";

bool operator == (const std::string & str, const Tracer & tracer){
	return tracer == str;
}
bool operator != (const std::string & str, const Tracer & tracer){
	return !(str == tracer);
}

Tracer TracerFactory::__tracer;
Tracer TracerFactory::__expect;

}// end of namespace unittest
}// end of namespace helper
