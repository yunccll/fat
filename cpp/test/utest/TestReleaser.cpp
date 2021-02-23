#include "gtest/gtest.h"

#include "helper/unittest/TestObjects.h"

#include "helper/Releaser.h"

using namespace helper;
using namespace unittest;

template<typename T>
void global_delete(T * obj){
    std::cout << " global_delete:" << obj << std::endl;
    delete obj;
}
TEST(CustomizeReleaserTest, testGlobalFunction){
    EXPECT.reset().ctor().dtor();
    {
        CustomizeReleaser<ObjectAsRef> releaser(&global_delete<ObjectAsRef>);
        releaser.release(new ObjectAsRef(TRACER.reset()));
        ASSERT_EQ(EXPECT, TRACER);
    }
}

template<typename T>
struct Deleter {
    void deleter(T * obj){
        std::cout << " Deleter::deleter" << obj << std::endl;
        delete obj;
    }
};
TEST(CustomizeReleaserTest, testMemberFunction){
    EXPECT.reset().ctor().dtor();
    {
        Deleter<ObjectAsRef> deleter;
        CustomizeReleaser<ObjectAsRef> releaser(std::bind(&Deleter<ObjectAsRef>::deleter, &deleter, std::placeholders::_1));

        releaser.release(new ObjectAsRef(TRACER.reset()));
        ASSERT_EQ(EXPECT, TRACER);
    }
}

TEST(CustomizeReleaserTest, testLambdaFunction){
    EXPECT.reset().ctor().dtor();
    {
        CustomizeReleaser<ObjectAsRef> releaser( [](ObjectAsRef *obj){
            std::cout << "lambd :"  << obj << std::endl;
            delete obj;
        });

        releaser.release(new ObjectAsRef(TRACER.reset()));
        ASSERT_EQ(EXPECT, TRACER);
    }
}


template<typename T>
class AutoReleaseObjectPtr {
public:
    AutoReleaseObjectPtr(T * ptr) 
    : releaser(new DefaultReleaser<T>)
    , ptr(ptr) 
    {
    }
    ~AutoReleaseObjectPtr(){
        releaser->release(ptr);
        delete releaser;
    }
private:
    IReleasable<T> * releaser;
    T * ptr;
};
TEST(RefactorMemPtrTest, testAutoReleaseObjectPtr){
    AutoReleaseObjectPtr<int> deleter(new int(10));

    EXPECT.reset().ctor().dtor();
    {
        AutoReleaseObjectPtr<ObjectAsRef> deleter(new ObjectAsRef(TRACER.reset()));
    }
    ASSERT_EQ(EXPECT, TRACER);
}



template<typename T>
class CustomizeReleaseObjectPtr {
public:
    CustomizeReleaseObjectPtr(T * obj, const std::function<void (T*)> & deleter)
    : releaser(new CustomizeReleaser<T>(deleter))
    , obj(obj)
    {
    }
    ~CustomizeReleaseObjectPtr() {
        releaser->release(obj);
        delete releaser;
    }
private:
    IReleasable<T> * releaser;
    T * obj;
};

TEST(CustomizeReleaseObjectPtrTest, testGlobalFunction){
    CustomizeReleaseObjectPtr<int> obj(new int(10), &global_delete<int>);

    EXPECT.reset().ctor().dtor();

    {
        CustomizeReleaseObjectPtr<ObjectAsRef> deleter(new ObjectAsRef(TRACER.reset()), &global_delete<ObjectAsRef>);
    }
    ASSERT_EQ(EXPECT, TRACER);
}

TEST(CustomizeReleaseObjectPtrTest, testLambdaFunction){
    EXPECT.reset().ctor().dtor();

    {
        CustomizeReleaseObjectPtr<ObjectAsRef> deleter(new ObjectAsRef(TRACER.reset()), [](ObjectAsRef * obj){
            std::cout << "lambda:" << obj << std::endl;
            delete obj;
        });
    }
    ASSERT_EQ(EXPECT, TRACER);
}
