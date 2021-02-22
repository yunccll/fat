#include "helper/object/ObjectInterface.h"
#include "helper/object/ObjectFactory.h"

namespace helper {
namespace object {

Object::Object(Object::Type type)
{ 
    type_ = type;
}
Object::~Object(){
}


String::String()
: Object(Object::STRING)
{
}
String::~String(){
}


Array::Array()
: Object(Object::ARRAY)
{
}
Array::~Array(){
}

Hash::Hash()
:Object(Object::HASH)
{
}
Hash::~Hash(){
}

Byte::Byte()
:Object(Object::BYTE)
{
}
Byte::~Byte(){
}
Integer::Integer()
:Object(Object::INTEGER)
{
}
Integer::~Integer(){
}
Double::Double()
:Object(Object::DOUBLE)
{
}
Double::~Double(){
} 


ObjectFactory  *  DefaultObjectFactory::factory = NULL;

DefaultObjectFactory::DefaultObjectFactory(){
    if(DefaultObjectFactory::factory == NULL){
        factory = new StandardObjectFactory();
    }
}
DefaultObjectFactory::~DefaultObjectFactory(){
}

} // end of namespace object
} // end of namespace helper

