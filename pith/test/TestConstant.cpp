#include <Pith/Constant.hpp>
#include <gtest/gtest.h>

using namespace Pith;

TEST(TestConstant, bool) {
	auto b = TrueConstant::VALUE;
	EXPECT_TRUE(b);
	EXPECT_FALSE(FalseConstant{}());
}

TEST(TestConstant, int) {
	auto i = IntConstant<2>::VALUE;
	EXPECT_EQ(i, 2);
	EXPECT_EQ(IntConstant<3>{}(), 3);
}
