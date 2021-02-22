#ifndef  HELPER_OBJECT_OBJECTINTERFACE_H 
#define  HELPER_OBJECT_OBJECTINTERFACE_H 

#include <string> 
#include <memory> 

namespace helper {
namespace object {

/**
 * Common Object interface for all data objects of GTP
 * 
 * @author Cheng Lun
 * @email yunccll@sina.com
 * @since 0.1
 */
class Object {
public:
    /**
     *  Base GTP Elements, STRING , ARRAY, HASH
     */
    enum Type{
        INTEGER = 1,
        DOUBLE,
        STRING,
        ARRAY,
        BYTE,
        HASH
    };

    /**
     * @param type   the type of the Object created
     */
    Object(Object::Type type);
    virtual ~Object();

    /**
     * @return true if the type of object is Array or false
     */
    bool isArray() const { return type_ == ARRAY;}
    /**
     * @return true if the type of object is Hash or false
     */
    bool isHash() const {return type_ == HASH;}
    /**
     * @return true if the type of object is String or false
     */
    bool isString() const {return type_ == STRING;}
    /**
     * @return true if the type of object is Byte or false
     */
    bool isByte() const {return type_ == BYTE;}
    /**
     * @return true if the type of object is INTEGER (64bit) or false
     */
    bool isInteger() const {return type_ == INTEGER;}
    /**
     * @return true if the type of object is Double (64bit) or false
     */
    bool isDouble() const {return type_ == DOUBLE;}

    /**
     * @return the real type of the object
     */
    Type getType() const {return type_;}

    /**
     * the string format of the object
     */
    virtual std::string toString() const = 0;

    virtual int compare(const Object * obj) const = 0; 

    virtual Object * copy() const = 0;
private:
    Type type_;
};

class Byte : public Object {
public:
    Byte();
    ~Byte() override;

    virtual const char set(const char value) = 0;
    virtual const char get() const = 0;
};
class Integer : public Object {
public:
    Integer();
    ~Integer() override;

    virtual const long long set(const long long value) = 0;
    virtual const long long get() const = 0;
};
class Double : public Object {
public:
    Double();
    ~Double() override;

    virtual const double set(const double value) = 0;
    virtual const double get() const = 0;
};

/**
 * String data interface for GTP elements
 *
 * @since 0.1
 */
class String : public Object{
public:
    String();
    ~String() override;

    /**
     * @param value  set the std::string value
     */
    virtual void setValue(const std::string & value) = 0;
    /**
     * @return  get the std::string & for value
     */
    virtual const std::string & getValue() const = 0;
    /**
     * @return the length of value with char(byte)
     */
    virtual int size() const = 0;

    /**
     * append the character to value 
     * @param ch character to be appended
     */
    virtual void append(char ch) = 0;
    /**
     * append the std::string to value
     * @param str std::string to be appended
     */
    virtual void append(const std::string & str) = 0;

};


/**
 * Iterator interface
 * @param <ValueType>  element type of Iterator
 *
 * @since 0.1
 */
template<typename ValueType>
class Iterator {
public:
    typedef ValueType ElementType;

    Iterator(){}
    virtual ~Iterator(){}

    /**
     * test the iterator has next element
     * @return true if the next element is not null
     */
    virtual bool hasNext() const = 0;
    /**
     * get the next element 
     * @return const value ref of the next element
     */
    virtual const ElementType & next() = 0;

    virtual bool __isOutOfRange(const ElementType & value) const {
        return &nullObj == &value;
    }

protected:
    virtual ValueType & __getNullObjRef() const {
        return nullObj;
    }

private:
    static ValueType nullObj;
};

template<typename ValueType>
ValueType Iterator<ValueType>::nullObj;


/**
 *  abstract Array interface for GTP element type
 */
class Array : public Object{
public:
    /// value type of Array element (pointer)
    typedef Object * ValueType;

    Array();
    /// free the memory of ValueType(Object *)
    ~Array() override;


    /**
     * @return the length of array
     */
    virtual int size() const = 0;

    /**
     * @param o append the value ptr to array
     */
    virtual void append(const ValueType &o) = 0;

    /**
     * <p> get the pointer of element in index (0-based)
     * @return value type (Object *) of array
     */
    virtual const ValueType * get(const int index) const = 0;

    const ValueType * operator[](const int index) const{
        return get(index);
    }

    /**
     * @return the iterator of array,  smart pointer to iterator object, free by myself
     */
    virtual std::shared_ptr<Iterator<ValueType>> iterator() const = 0;

    /** 
     *
     * the memory of old value is managed by user
     * <pre>{@code
     *      Object * old = this.remove(0);
     *      delete old;
     * }</pre>
     *
     * <p> just remove the value(Objet *) in the private container
     * @param index the index of element in array
     */
    virtual ValueType remove(const int index) = 0;

    /**
     * <p> set the value to index in the container
     * nothing to do for old value 
     *
     *<pre>{@code
     *      Object * new = new Object();
     *      Object * old = this.set(0, new);
     *      if(old != nullptr)
     *          delete old;
     *      else
     *          delete new;
     *}</pre>
     *
     * @param index index(0-based)
     * @param value the new value
     */
    virtual ValueType set(const int index, const ValueType & value) = 0;
};

/** 
 *  abstract Hash interface for GTP element type
 */
class Hash : public Object{
public:
    typedef std::pair<const std::string, Object *> Pair;

    Hash();
    ~Hash() override;

    /**
     * @return the length of Hash
     */
    virtual int size() const = 0;

    /**
     * <p> get the reference of Pair with key
     * @return value type (Object *) of hash
     */
    virtual const Pair::second_type * get(const Pair::first_type  & key) const = 0;

    const Pair::second_type * operator[](const Pair::first_type & key) const {
        return get(key);
    }

    /** 
     * contain the key or not
     * @return true if the key is in the hash or false
     */
    virtual bool containsKey(const Pair::first_type & key) const = 0;

    /** 
     * insert the key & value into hash
     * @param key the std::string as key
     * @param value the std::string as value
     */
    virtual int insert(const Pair::first_type & key, const std::string & value) = 0;
    /**
     * insert the key & value into hash
     * @param key the std::string as Key
     * @param value the Object * as value 
     */
    virtual int insert(const Pair::first_type & key, const Pair::second_type & o) = 0;
    /**
     * @return the iterator of array,  smart pointer to iterator object, free by myself
     */
    virtual std::shared_ptr<Iterator<Hash::Pair>> iterator() const = 0;


    /** 
     *
     * the memory of old value is managed by user
     * <pre>{@code
     *      Object * old = this.remove("key")
     *      delete old;
     * }</pre>
     *
     *
     * <p> just remove the value (Objet *) in the private container
     * @param key the key of element in hash
     */
    virtual Pair::second_type remove(const Pair::first_type & key) = 0;
    /**
     * <p> set the new value to the hash with key
     * nothing effect to old value 
     *
     *<pre>{@code
     *      Object * new = new Object();
     *      Object * old = this.set("key", new);
     *      if(old != nullptr)
     *          delete old;
     *      else 
     *          delete new;
     *}</pre>
     *
     * @param key the std::string as key
     * @param value the Object *  as value
     */
    virtual Pair::second_type set(const Pair::first_type & key, Pair::second_type & value) = 0;
};



class ObjectFactory {
public:
    ObjectFactory(){}
    virtual ~ObjectFactory(){}

    inline std::shared_ptr<String> createStupidString(){
        return std::shared_ptr<String>(createString());
    }
    inline std::shared_ptr<String> createStupidString(const std::string & str){
        return std::shared_ptr<String>(createString(str));
    }
    inline std::shared_ptr<Array> createStupidArray(){
        return std::shared_ptr<Array>(createArray());
    }
    inline std::shared_ptr<Hash> createStupidHash(){
        return std::shared_ptr<Hash>(createHash());
    }
    inline std::shared_ptr<Byte> createStupidByte(const char val){
        return std::shared_ptr<Byte>(createByte(val));
    }
    inline std::shared_ptr<Integer> createStupidInteger(const long long val){
        return std::shared_ptr<Integer>(createInteger(val));
    }
    inline std::shared_ptr<Double> createStupidDouble(const double val){
        return std::shared_ptr<Double>(createDouble(val));
    }

    virtual String * createString(const std::string & str) const = 0;
    virtual String * createString() const = 0;
    virtual Array * createArray() const = 0;
    virtual Hash * createHash() const = 0;

    virtual Byte * createByte(const char val) const = 0;
    virtual Integer * createInteger(const long long val) const = 0;
    virtual Double * createDouble(const double val) const = 0;

    virtual std::string toString() const = 0;
};

class DefaultObjectFactory {
private:
    DefaultObjectFactory();

public:
    ~DefaultObjectFactory();

    inline static ObjectFactory & instance() { 
        static DefaultObjectFactory defaultFactory;
        return *(defaultFactory.factory); 
    }

    static ObjectFactory  *  factory;
};

} // end of namespace object
} // end of namespace helper

#endif   /* HELPER_OBJECT_OBJECTINTERFACE_H */
