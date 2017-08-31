#include <Pith/Config.hpp>

#ifdef PITH_DISABLE

#include <Pith/VarInt.hpp>
#include <gtest/gtest.h>

TEST(TestVarInt, one) {
	Core::VarInt<1> i = 2;
	EXPECT_EQ(i, 2);
	EXPECT_EQ(i.value_[0], 2);
}

TEST(TestVarInt, one) {
	Core::VarInt<1> i = 2;
	EXPECT_EQ(i, 2);
	EXPECT_EQ(i.value_[0], 2);
}

#endif  // PITH_DISABLE
