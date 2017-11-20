#include <Ab/Config.hpp>

#ifdef AB_DISABLE

#include <Ab/VarInt.hpp>
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

#endif  // AB_DISABLE
