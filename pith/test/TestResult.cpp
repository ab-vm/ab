#include <Pith/Result.hpp>
#include <gtest/gtest.h>

using namespace Pith;

enum class MyValue { one, two };
enum class MyError { one, two };

TEST(TestOk, base) {
	Ok<int> ok1{IN_PLACE};
	ok1() = 1;
	EXPECT_EQ(ok1(), 1);

	Ok<int> ok2{2};
	EXPECT_EQ(ok2(), 2);

	Ok<int> ok3{IN_PLACE, 3};
	EXPECT_EQ(ok3(), 3);
}

TEST(TestResult, base) {
	Result<MyValue, MyError> r = err(MyError::one);
	EXPECT_EQ(r.err(), MyError::one);
	EXPECT_FALSE(r);
}

TEST(TestResult, ok) {
	Result<MyValue, MyError> r = ok(MyValue::one);
	EXPECT_EQ(r(), MyValue::one);
	EXPECT_TRUE(r);
}
