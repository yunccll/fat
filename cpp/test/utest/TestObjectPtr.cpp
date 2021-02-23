#include "gtest/gtest.h"


#include "helper/ObjectPtr.h"
using namespace helper;

TEST(ObjectPtrTest, testObjectPtr){
	double expectDouble = 20.0f;
	BaseObjectPtr<double> objptr(new double(expectDouble));
	ASSERT_TRUE(*objptr == expectDouble);
}

TEST(PinnableObjectPtrTest, test){
	int * ptr = nullptr;
	{
		PinnableObjectPtr<int> obj(new int(10));
		ptr = obj.pin();
	}
	ASSERT_TRUE(ptr != nullptr);
	ASSERT_EQ(10, *ptr);
    delete ptr;
}
