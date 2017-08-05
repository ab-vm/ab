#include <Om/Value.hpp>
#include <gtest/gtest.h>

using namespace Om;

TEST(Value, constructor) {
	Value x1 = 1;
	Value x2 = 1.0;
	Value x3 = nullptr;
}

TEST(Value, to) {
	Value x = 1;
	EXPECT_EQ(1, Value(1).to<int>());
}