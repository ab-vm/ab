#include <Pith/Result.hpp>
#include <gtest/gtest.h>

using namespace Pith;

enum class MyValue { one, two };
enum class MyError { one, two };

TEST(TestResult, base) {
	Result<MyValue, MyError> r = error(MyError::one);
	EXPECT_EQ(r.error(), MyError::one);
	r = ok(MyValue::one);
	EXPECT_EQ(r.value(), MyValue::one);
}
