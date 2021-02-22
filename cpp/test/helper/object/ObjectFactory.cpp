#include "helper/object/ObjectFactory.h"
#include "helper/object/StandardObject.h"

namespace helper {
namespace object {

std::shared_ptr<ObjectFactory> StandardObjectFactory::make(){
    return std::make_shared<StandardObjectFactory>();
}
StandardObjectFactory::StandardObjectFactory(){
}
StandardObjectFactory::~StandardObjectFactory(){
}

std::string StandardObjectFactory::toString() const{
    return "StandardObjectFactory";
}

String * StandardObjectFactory::createString() const{
    return new StandardString();
}
String * StandardObjectFactory::createString(const std::string & str) const{
    return new StandardString(str);
}
Array * StandardObjectFactory::createArray() const{
    return new StandardArray();
}
Hash * StandardObjectFactory::createHash() const{
    return new StandardHash();
}
Byte * StandardObjectFactory::createByte(const char val) const {
    return new StandardByte(val);
}
Integer * StandardObjectFactory::createInteger(const long long val) const {
    return new StandardInteger(val);
}
Double * StandardObjectFactory::createDouble(const double val) const {
    return new StandardDouble(val);
}

} // end of namespace object
} // end of namespace helper
