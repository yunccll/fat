#ifndef  HELPER_OBJECTPTR_H
#define  HELPER_ OBJECTPTR_H

#include <functional>
namespace helper {

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

    inline void reset(){
        ptr = nullptr;
    }
    template<typename V>
	inline void reset(V * v) {
		ptr = v;
	}

private:
	T * ptr;
};

template<typename T>
class IPinnable {
public:
	IPinnable():bPin(true){}
	explicit IPinnable(bool bPin) : bPin(bPin) {}
	virtual ~IPinnable(){}

	virtual T * pin() = 0;
	virtual void eject()  = 0;

	inline bool isPin() const {
		return bPin;
	}
	inline void setPin(bool bPin){
		this->bPin = bPin;
	}
private:
	bool bPin;
};

template<typename T>
class PinnableObjectPtr : public BaseObjectPtr<T> , public IPinnable<T> {
public:
	template<typename V>
	PinnableObjectPtr(V * obj)
	: BaseObjectPtr<T>(obj)
	, IPinnable<T>(false)
	{
	}
	template<typename V>
	PinnableObjectPtr(V * obj, bool bPin)
	: BaseObjectPtr<T>(obj)
	, IPinnable<T>(bPin)
	{
	}
	~PinnableObjectPtr()override {}
	
	T * pin() override {
        this->setPin(true);
        return this->get();
	}
    void eject() override {
        //not-support
    }
};


} // end of namespace helper


#endif   /* HELPER_OBJECTPTR_H */
