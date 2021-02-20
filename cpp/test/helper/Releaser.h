
#ifndef  HELPER_RELEASER_H
#define  HELPER_RELEASER_H

namespace helper {

template<typename T>
class IReleasable {
public:
	virtual ~IReleasable(){
	}
	virtual void release(T * obj) = 0;
};

template<typename T>
class DefaultReleaser : public IReleasable<T>{
public:
	~DefaultReleaser() override{
	}
	void release(T * obj) override {
        if(obj != nullptr)
            delete obj;
    }
};


template<typename T>
class CustomizeReleaser : public IReleasable<T> {
public: 
    CustomizeReleaser(const std::function<void (T*)> & deleter)
    : deleter(deleter)
    {
    }
    ~CustomizeReleaser() override{
    }
	void release(T * obj) override {
        if(deleter){
            deleter(obj);
        }
        else{
            //std::cout << "CustomizeReleaser :: delete " << std::endl;
            delete obj;
        }
    }
private:
    std::function<void (T*)> deleter;
};

} //end of namespace helper

#endif   /* HELPER_RELEASER_H */
