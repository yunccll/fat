
#ifndef  HELPER_OBJECT_OBJECTFACTORY_H
#define  HELPER_OBJECT_OBJECTFACTORY_H

#include <memory>
#include <string>
#include "helper/object/ObjectInterface.h"

namespace helper {
namespace object {

class StandardObjectFactory : public ObjectFactory{
public:
    static std::shared_ptr<ObjectFactory> make();

    StandardObjectFactory();
    ~StandardObjectFactory();

    std::string toString() const override;

    String * createString(const std::string & str) const override;
    String * createString() const override;
    Array * createArray() const override;
    Hash * createHash() const override;

    Byte * createByte(const char val) const override;
    Integer * createInteger(const long long val) const override;
    Double * createDouble(const double val) const override;
};

} // end of namespace object
} // end of namespace helper

#endif   /* HELPER_OBJECT_OBJECTFACTORY_H */
