#include "gtest/gtest.h"

//TODO: refactor the mem_ptr , ????? SRP
template<typename T>
class BaseObjectPtr {
public:
	template<typename V>
	BaseObjectPtr(V * ptr) : ptr(ptr) {}
	virtual ~BaseObjectPtr(){}

    inline T & operator *() {
        return *ptr;
    }
    inline T * operator ->(){
        return ptr;
    }
	inline T * get() const {
		return ptr;
	}
	inline bool empty() const {
		return ptr == nullptr;
	}
	inline void reset() {
		ptr = nullptr;
	}

private:
	T * ptr;
};

TEST(RefactorMemPtrTest, testObjectPtr){
	double expectDouble = 20.0f;
	BaseObjectPtr<double> objptr(new double(expectDouble));
	ASSERT_TRUE(*objptr == expectDouble);
}




template<typename T>
class IPinnable {
public:
	IPinnable():bPin(true){}
	explicit IPinnable(bool bPin) : bPin(bPin) {}
	virtual ~IPinnable(){}

	virtual T * pin(){ setPin(true); return nullptr;}
	virtual void unPin() { setPin(false); }

	bool isPin() const {
		return bPin;
	}
protected:
	void setPin(bool bPin){
		this->bPin = bPin;
	}
private:
	bool bPin;
};

template<typename T>
class PinnalbeObjectPtr : public BaseObjectPtr<T> , public IPinnable<T> {
public:

	template<typename V>
	PinnalbeObjectPtr(V * obj)
	: BaseObjectPtr<T>(obj)
	, IPinnable<T>(false)
	{
	}
	~PinnalbeObjectPtr()override {}
	
	T * pin() override {
		IPinnable<T>::setPin(true);
		return BaseObjectPtr<T>::get();
	}
};


TEST(RefactorMemPtrTest, testPinnable){
	IPinnable<int> pin(false);
	ASSERT_TRUE(false == pin.isPin());
	ASSERT_TRUE(nullptr == pin.pin());
	ASSERT_TRUE(true == pin.isPin());

	int * ptr = nullptr;
	{
		PinnalbeObjectPtr<int> obj(new int(10));
		ptr = obj.pin();
	}
	ASSERT_TRUE(ptr != nullptr);
	ASSERT_EQ(10, *ptr);
}



//TODO: 
// release -> default Release, Pinnable Releaser, Customize Releaser ....
template<typename T>
class IReleasable {
public:
	virtual ~IReleasable(){
	}
	virtual void release(T * obj) {
		std::cout << "destructor : " << *obj << std::endl;
		delete obj;
	}
};
template<typename T>
class ReleasableObjectPtr : public BaseObjectPtr<T> {
public:
	template<typename V>
	ReleasableObjectPtr(V * obj) 
	: BaseObjectPtr<T>(obj) 
	, releaser(new IReleasable<T>)
	{
	}
	virtual ~ReleasableObjectPtr(){
		releaser->release(BaseObjectPtr<T>::get());
	}
private:
	IReleasable<T> * releaser;
};

TEST(RefactorMemPtrTest, testReleaseObject){
	ReleasableObjectPtr<int> ptr(new int(10));
	ASSERT_EQ(10, *ptr);
}

//	inline bool operator == (const ObjectPtr<T> & objptr) const {
//		return ptr == objptr.ptr;
//	}
//	inline bool operator !=(const ObjectPtr<T> & objptr) const {
//		return !(*this == objptr);
//	}
//template<typename T>
//bool operator == (const void * ptr, const ObjectPtr<T> & objptr){
//	return ptr == objptr.get();
//}
//template<typename T>
//bool operator != (const void * ptr, const ObjectPtr<T> & objptr){
//	return !(ptr == objptr);
//}
