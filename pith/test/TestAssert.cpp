#include <Pith/Config.hpp>
#include <Pith/Assert.hpp>
#include <gtest/gtest.h>

TEST(Assert, fail) {
	ASSERT_DEATH(PITH_ASSERT(false), "Error");
}

TEST(Assert, unreachable) {
	ASSERT_DEATH(PITH_ASSERT_UNREACHABLE(), "Error");
}

TEST(Assert, success) {
	EXPECT_EXIT({
		PITH_ASSERT(true);
		std::cerr << "Success" << std::endl;
		exit(0);
	},
	testing::ExitedWithCode(0), "Success");
}
