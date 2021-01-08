
#ifndef  UNIT_TEST_WRAPPER_H
#define  UNIT_TEST_WRAPPER_H


#include <assert.h>




#define TEST(test_case, func_name)  void test_case##_##func_name(void)

#define ASSERT_EQ(expect, expr)     assert(expect == (expr))
#define ASSERT_TRUE(expr)           assert(expr)


#endif   /* UNIT_TEST_WRAPPER_H */
