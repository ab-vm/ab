#include <Pith/Array.hpp>
#include <gtest/gtest.h>

using namespace Pith;

#if 0
TEST(TestArray, intArrayOne) {
	Array<int, 1> b;
	b.value_[0] = 42;
	EXPECT_EQ(b.value_[0], 42);
}

TEST(TestArray, intTwo) {
	Array<int, 2> b;
	EXPECT_EQ(b.value_[0] = 0, 0);
	EXPECT_EQ(b.value_[1] = 1, 1);
}

TEST(TestArray, byteArrayOne) {
	ByteArray<1> b;
	b.value_[0] = 42;
	EXPECT_EQ(b.value_[0], 42);
}

TEST(TestArray, byteArrayTwo) {
	ByteArray<2> b;
	EXPECT_EQ(b.value_[0] = 0, 0);
	EXPECT_EQ(b.value_[1] = 1, 1);
}
#endif // 0
