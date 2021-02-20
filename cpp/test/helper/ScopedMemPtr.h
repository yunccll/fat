#ifndef  HELPER_SCOPEDMEMPTR_H
#define  HELPER_SCOPEDMEMPTR_H

#include "helper/ObjectPtr.h"
#include "helper/Releaser.h"

namespace helper {

template<typename T>
class ScopedMemPtr : public PinnableObjectPtr<T> {
public:
    ScopedMemPtr()
    :ScopedMemPtr<T>((T*)(nullptr), true, nullptr)
    {
    }
    ScopedMemPtr(std::nullptr_t )
    :ScopedMemPtr<T>((T*)nullptr, true, nullptr)
    {
    }
    
	//move constructor
	ScopedMemPtr(ScopedMemPtr<T> && mem_obj) 
    : ScopedMemPtr()
	{
        moveOwner(*this, mem_obj);
	}

    template<typename V>
    ScopedMemPtr(V * obj,  bool canReleased = true, IReleasable<T> * releaser = nullptr)
    : PinnableObjectPtr<T>(obj, !canReleased)
    , releaser(releaser == nullptr ? new DefaultReleaser<T> : releaser)
    {
    }

    ~ScopedMemPtr() override {
        if(!this->isPin() && releaser != nullptr){
            releaser->release(this->get());
        }
    }

	bool empty() const {
		return this->get() == nullptr;
	}

public:
	inline bool operator == (const ScopedMemPtr<T> & mptr) const {
		return this->get() == mptr.get();
	}
	inline bool operator !=(const ScopedMemPtr<T> & mptr) const {
		return !(*this == mptr);
	}

public:
	ScopedMemPtr<T> & operator = (ScopedMemPtr<T> && mem_obj){
        moveOwner(*this, mem_obj);
		return *this;
	}

private:
    void moveOwner(ScopedMemPtr<T> & left, ScopedMemPtr<T> & right){
		if(!left.empty()){
			left.releaser->release(left.get());
            delete left.releaser; // dependent to new DefaultReleaser
		}
        left.releaser = right.releaser;
        right.releaser = nullptr;

        left.reset(right.get()); 
        right.reset();

        left.setPin(right.isPin());
        right.setPin(false);


    }

private:
	ScopedMemPtr(const ScopedMemPtr<T> & mem_obj) = delete;
	ScopedMemPtr<T> & operator =(const ScopedMemPtr<T> & mem_obj) = delete;

private:
    IReleasable<T> * releaser;
};

template<typename T>
bool operator == (const T * ptr, const ScopedMemPtr<T> & mptr){
	return ptr == mptr.get();
}
template<typename T>
bool operator != (const T * ptr, const ScopedMemPtr<T> & mptr){
	return !(ptr == mptr);
}

template<typename T>
bool operator == (const std::nullptr_t * ptr, const ScopedMemPtr<T> & mptr){
	return (const T*)(ptr) == mptr.get();
}
template<typename T>
bool operator != (const std::nullptr_t * ptr, const ScopedMemPtr<T> & mptr){
	return !(ptr == mptr);
}



template<typename T>
helper::ScopedMemPtr<T> make_scoped(){
    return helper::ScopedMemPtr<T>(new T());
}

template<typename T, typename Arg1>
helper::ScopedMemPtr<T> make_scoped(const Arg1 & arg1){
    return helper::ScopedMemPtr<T>(new T(arg1));
}
template<typename T, typename Arg1, typename Arg2>
helper::ScopedMemPtr<T> make_scoped(const Arg1 & arg1, const Arg2 & arg2){
    return helper::ScopedMemPtr<T>(new T(arg1, arg2));
}
template<typename T, typename Arg1, typename Arg2, typename Arg3>
helper::ScopedMemPtr<T> make_scoped(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3){
    return helper::ScopedMemPtr<T>(new T(arg1, arg2, arg3));
}
template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
helper::ScopedMemPtr<T> make_scoped(const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4){
    return helper::ScopedMemPtr<T>(new T(arg1, arg2, arg3, arg4));
}

template<typename T, typename Arg1>
helper::ScopedMemPtr<T> make_scoped(Arg1 & arg1){
    return helper::ScopedMemPtr<T>(new T(arg1));
}
template<typename T, typename Arg1, typename Arg2>
helper::ScopedMemPtr<T> make_scoped(Arg1 & arg1, Arg2 & arg2){
    return helper::ScopedMemPtr<T>(new T(arg1, arg2));
}
template<typename T, typename Arg1, typename Arg2, typename Arg3>
helper::ScopedMemPtr<T> make_scoped(Arg1 & arg1, Arg2 & arg2, Arg3 & arg3){
    return helper::ScopedMemPtr<T>(new T(arg1, arg2, arg3));
}
template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
helper::ScopedMemPtr<T> make_scoped(Arg1 & arg1, Arg2 & arg2, Arg3 & arg3, Arg4 & arg4){
    return helper::ScopedMemPtr<T>(new T(arg1, arg2, arg3, arg4));
}

} //end of namespace helper;

#endif   /* HELPER_SCOPEDMEMPTR_H */
