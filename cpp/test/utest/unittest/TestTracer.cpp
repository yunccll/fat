#include "gtest/gtest.h"
#include <memory>

#include "helper/unittest/Tracer.h"
#include "helper/unittest/TestObjects.h"

using namespace helper;
using namespace unittest;

TEST(TracerTest, testGlobalUse){
    TRACER.reset();
    EXPECT.reset();
    ASSERT_EQ(EXPECT, TRACER);

    EXPECT.ctor();
    TRACER.ctor();
    ASSERT_EQ(EXPECT, TRACER);

    EXPECT.dtor();
    TRACER.dtor();
    ASSERT_EQ(EXPECT, TRACER);
}

TEST(TracerTest, testObjectAsMember){
    EXPECT.reset().ctor().method("set").methodConst("get");

	ObjectAsMember obj;  
    obj.set().get();
    ASSERT_EQ(EXPECT, obj.getTracer());
}

TEST(TracerTest, testObjectAsRef){
    EXPECT.reset().ctor().method("set").method("get").dtor();

	{
		ObjectAsRef obj(TRACER.reset()); 
        obj.set().get();
	}
    ASSERT_EQ(EXPECT, TRACER);
}

TEST(TracerTest, testObjectAsBase){
    EXPECT.reset().ctor().method("set").methodConst("get");

	ObjectAsBase obj;
	obj.set().get();
    ASSERT_EQ(EXPECT, obj.ref());
}


TEST(TracerTest, testTracerWithInitString){
    {
        Tracer t1("id");
        Tracer t2("id2");
        ASSERT_NE(t1, t2);
    }
    {
        Tracer t1("id1");
        ObjectAsRef obj(t1);

        Tracer t2("id2");
        ObjectAsRef obj2(t2); 

        ASSERT_NE(t1, t2);
    }
}
