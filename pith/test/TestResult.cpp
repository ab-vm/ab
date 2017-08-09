#include <Pith/Result.hpp>
#include <gtest/gtest.h>

using namespace Pith;

enum class MyValue { one, two };
enum class MyError { one, two };

TEST(TestResult, base) {
	Result<MyValue, MyError> r = err<MyError>(MyError::one);
	EXPECT_EQ(r.error(), MyError::one);
	EXPECT_FALSE(r);
	r = ok<MyValue, MyValue>(MyValue::one);
	EXPECT_EQ(r.value(), MyValue::one);
	EXPECT_TRUE(r);
}
