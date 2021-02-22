#include "helper/object/StandardObject.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iomanip>

namespace helper {
namespace object {


StandardString::StandardString()
: String()
{
}

StandardString::StandardString(const std::string & str)
: String()
, value_(str)
{
}

StandardString::~StandardString(){
}

void StandardString::setValue(const std::string & value){
    if(value != this->value_){
        this->value_ = value;
    }
}
const std::string & StandardString::getValue() const{
    return value_;
}

int StandardString::size() const {
    return (int)value_.size();
}

void StandardString::append(const std::string & str){
    value_ += str;
}
void StandardString::append(char ch){
    value_.push_back(ch);
}

std::string StandardString::toString() const{
    return "StandardString:" + value_;
}

int StandardString::compare(const Object * obj) const{
    if (obj == nullptr) {
        return -1;
    }

    if (!obj->isString()) {
        return -1;
    }

    return value_.compare((static_cast<const StandardString *>(obj))->getValue()); 
}

Object * StandardString::copy() const {
    return new StandardString(value_);
}

StandardArray::StandardArray(){
}
StandardArray::~StandardArray(){
    freeHolderResource();
}

int StandardArray::size() const{
    return array.size();
}
void StandardArray::append(const Array::ValueType & o){
    array.push_back(o);
}

const Array::ValueType * StandardArray::get(const int index) const{
    if(index >= 0 && index < size()){
        return &(array.at(index));
    }
    return nullptr;
}
void StandardArray::freeHolderResource(){
    for(auto iter = array.begin(); iter != array.end(); ++iter){
        auto ptr = *iter;
        if(ptr != nullptr){
            delete ptr; //TODO: free by himself
            *iter = nullptr;
        }
    }
}

Array::ValueType StandardArray::remove(const int index){
    if(index >= 0 && index < size()){
        Array::ValueType old = array.at(index);
        array.erase(array.begin() + index);
        return old;
    }
    return nullptr;
} 

Array::ValueType StandardArray::set(const int index, const ValueType & value){
    if(index >= 0 && index < size()){
        Array::ValueType old = array.at(index);
        array[index] = value;
        return old;
    }
    return nullptr;
}

std::string StandardArray::toString() const{
    std::ostringstream oss;

    oss << "StandardArray:[" << array.size() << "] : ";
    for(auto iter = array.begin(); iter != array.end(); ++iter){
        std::string val = (*iter) != nullptr ? (*iter)->toString() : "nullptr";
        oss << "item:" <<  val << ", ";
    }
    return oss.str();
}

int StandardArray::compare(const Object * obj) const {
    if (obj == nullptr) {
        return -1;
    }

    if (!obj->isArray()) {
        return -1;
    }

    if (this == obj) {
        return 0;
    }

    const StandardArray * arrayObj = static_cast<const StandardArray *>(obj);
    if (size() != arrayObj->size()) {
        return -1;
    }

    auto it = arrayObj->iterator();
    for(auto iter = array.begin(); iter != array.end() && it->hasNext(); ++iter) {
        const Object * o = it->next();
        if ((*iter) == o) {
            continue;
        }
        else {
            if ((*iter) == nullptr) {
                return -1;
            }

            if (0 == (*iter)->compare(o)) {
                continue;
            }
            else {
                return -1;
            }
        }
    }
    return 0;
}

Object * StandardArray::copy() const {
    StandardArray * newArray =  new StandardArray();
    for(auto iter = array.begin(); iter != array.end(); ++iter) {
        if ((*iter) != nullptr) {
            newArray->append((*iter)->copy());
        }
        else {
            newArray->append(nullptr);
        }
    }
    return newArray;
}

std::shared_ptr<Iterator<Array::ValueType>> StandardArray::iterator() const{
    return std::make_shared<StandardArrayIterator>(*this);
}

StandardArrayIterator::StandardArrayIterator(Array & array)
: arrayRef(array)
, index(0)
{
}

StandardArrayIterator::StandardArrayIterator(const Array & array)
: arrayRef(array)
, index(0)
{
}
StandardArrayIterator::~StandardArrayIterator(){
}

bool StandardArrayIterator::hasNext() const{
    if(index >= 0 && index < arrayRef.size()){
        return true;
    }
    return false;
}
const Array::ValueType & StandardArrayIterator::next(){
    if(index >= 0 && index < arrayRef.size()){
        StandardArray & sa = (StandardArray &)arrayRef;
        return sa.array.at(index++);
    }
    return __getNullObjRef();
}


StandardHash::StandardHash(){
}
StandardHash::~StandardHash(){
    freeHolderResource();
}

int StandardHash::size() const{
    return hash.size();
}
const Hash::Pair::second_type * StandardHash::get(const std::string  & key) const{
    if(containsKey(key)){
        return &(hash.at(key));
    }
    return nullptr;
}

bool StandardHash::containsKey(const std::string & key) const{
    return hash.find(key) != hash.end();
}
int StandardHash::insert(const std::string & key, const std::string & value){
    StandardString * data = new StandardString(value);
    int ret = insert(key, data);
    if (0 == ret) {
        return 0;
    }
    else {
        delete data;
        return -1;
    }
}
int StandardHash::insert(const std::string & key, const Hash::Pair::second_type & o){
    auto ret = hash.insert(HashImp::value_type(key, o));
    return ret.second ? 0 : -1;
}

void StandardHash::freeHolderResource(){
    for(auto iter = hash.begin(); iter != hash.end(); ++iter){
        Object * ptr = iter->second;
        if(ptr != nullptr){
            delete ptr;
            hash[iter->first] = nullptr;
        }
    }
}
std::shared_ptr<Iterator<Hash::Pair>> StandardHash::iterator() const{
    return std::make_shared<StandardHashIterator>(*this);
}
Hash::Pair::second_type StandardHash::remove(const Hash::Pair::first_type & key) {
    auto iter = hash.find(key); 
    if(iter != hash.end()){
        Hash::Pair::second_type old = iter->second;
        hash.erase(iter);
        return old;
    }
    return nullptr;
}
Hash::Pair::second_type StandardHash::set(const Hash::Pair::first_type & key, Hash::Pair::second_type & value){
    Hash::Pair::second_type old = nullptr; 
    auto iter = hash.find(key);
    if(iter != hash.end()){
        old = iter->second;
        hash.erase(iter);
    }
    insert(key, value);
    return old;
}

std::string StandardHash::toString() const{
    std::ostringstream oss;
    oss << "StandardHash:[" << hash.size() << "] : ";
    for(auto iter = hash.begin(); iter != hash.end(); ++iter){
        std::string val = (iter->second != nullptr) ? iter->second->toString() : "nullptr";
        oss << iter->first << ":" << val << ", ";
    }
    return oss.str();
}

int StandardHash::compare(const Object * obj) const {
    if (obj == nullptr) {
        return -1;
    }

    if (!obj->isHash()) {
        return -1;
    }

    if (this == obj) {
        return 0;
    }

    const StandardHash * hashObj = static_cast<const StandardHash *>(obj);
    if (size() != hashObj->size()) {
        return -1;
    }
    
    auto iter = iterator();
    while (iter->hasNext()) {
        const Hash::Pair & p = iter->next();
        if (hashObj->containsKey(p.first)) {
            if (0 != p.second->compare(*(hashObj->get(p.first)))) {
                return -1;
            }
        }
        else {
            return -1;
        }
    }
    return 0;
}

Object * StandardHash::copy() const {
    StandardHash * newHash = new StandardHash();
    auto iter = iterator();
    while (iter->hasNext()) {
        const Hash::Pair & p = iter->next();
        if (p.second != nullptr) {
            newHash->insert(p.first, p.second->copy());
        }
        else {
            newHash->insert(p.first, nullptr);
        }
    }
    return newHash;
}

StandardHashIterator::StandardHashIterator(Hash & hash)
: hashRef(hash)
{
    StandardHash & sh = (StandardHash &)(hashRef);
    iter = sh.hash.begin();
    end = sh.hash.end();
}

StandardHashIterator::StandardHashIterator(const Hash & hash)
: hashRef(hash)
{
    StandardHash & sh = (StandardHash &)(hashRef);
    iter = sh.hash.begin();
    end = sh.hash.end();
}

StandardHashIterator::~StandardHashIterator(){
}

bool StandardHashIterator::hasNext() const{
    return iter != end;
}

const Hash::Pair & StandardHashIterator::next(){
    //const Hash::Pair & ret = *iter;
    //++iter;
    return *(iter++);
}

StandardByte::StandardByte(const char value)
:value_(value)
{
}
StandardByte::~StandardByte(){
}

const char StandardByte::set(const char value){
    const char old = value;
    this->value_ = value;
    return old;
}
const char StandardByte::get() const {
    return value_;
}
std::string StandardByte::toString() const {
    std::ostringstream oss;
    oss << "StandardByte:"<< value_;
    return oss.str();
}

int StandardByte::compare(const Object * obj) const {
    if (obj == nullptr) {
        return -1;
    }

    if (!obj->isByte()) {
        return -1;
    }

    if (this == obj) {
        return 0;
    }

    return (value_ - (static_cast<const StandardByte *>(obj))->get());
}

Object * StandardByte::copy() const {
    return new StandardByte(value_);
}


StandardInteger::StandardInteger(const long long value)
: value_(value)
{
}
StandardInteger::~StandardInteger() {
}

const long long StandardInteger::set(const long long value) {
    const long long old = value;
    this->value_ = value;
    return old;
}
const long long StandardInteger::get() const {
    return value_;
}
std::string StandardInteger::toString() const {
    std::ostringstream oss;
    oss << "StandardInteger:"<< value_;
    return oss.str();
}

int StandardInteger::compare(const Object * obj) const {
    if (obj == nullptr) {
        return -1;
    }

    if (!obj->isInteger()) {
        return -1;
    }

    if (this == obj) {
        return 0;
    }

    return (value_ - (static_cast<const StandardInteger *>(obj))->get());
}

Object * StandardInteger::copy() const {
    return new StandardInteger(value_);
}


StandardDouble::StandardDouble(const double value)
: value_(value)
{
}
StandardDouble::~StandardDouble(){
}

const double StandardDouble::set(const double value) {
    const double old = value;
    this->value_ = value;
    return old;
}
const double StandardDouble::get() const {
    return value_;
}

std::string StandardDouble::toString() const {
    std::ostringstream oss;
    oss << "StandardDouble:" << std::fixed << std::setprecision(9) << value_;
    return oss.str();
}

int StandardDouble::compare(const Object * obj) const {
    if (obj == nullptr) {
        return -1;
    }

    if (!obj->isDouble()) {
        return -1;
    }

    if (this == obj) {
        return 0;
    }

    return (value_ - (static_cast<const StandardDouble *>(obj))->get()) == 0? 0 : -1;
}

Object * StandardDouble::copy() const {
    return new StandardDouble(value_);
}

} // end of namespace object
} // end of namespace helper
