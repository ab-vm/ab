#include <Pith/ArrayBox.hpp>
#include <gtest/gtest.h>

using namespace Pith;

#if 0
/// TODO: Write ArrayBox tests

TEST(TestArrayBox, intArrayOne) {
	Array<int, 1> b;
	b.value_[0] = 42;
	EXPECT_EQ(b.value_[0], 42);
}

TEST(TestArrayBox, intTwo) {
	Array<int, 2> b;
	EXPECT_EQ(b.value_[0] = 0, 0);
	EXPECT_EQ(b.value_[1] = 1, 1);
}

TEST(TestArrayBox, byteArrayOne) {
	ByteArray<1> b;
	b.value_[0] = 42;
	EXPECT_EQ(b.value_[0], 42);
}

TEST(TestArrayBox, byteArrayTwo) {
	ByteArray<2> b;
	EXPECT_EQ(b.value_[0] = 0, 0);
	EXPECT_EQ(b.value_[1] = 1, 1);
}

#endif  // 0