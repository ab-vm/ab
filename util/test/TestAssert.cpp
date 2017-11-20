#include <Ab/Config.hpp>
#include <Ab/Assert.hpp>
#include <gtest/gtest.h>

TEST(Assert, fail) {
	ASSERT_DEATH(AB_ASSERT(false), "Error");
}

TEST(Assert, unreachable) {
	ASSERT_DEATH(AB_ASSERT_UNREACHABLE(), "Error");
}

TEST(Assert, success) {
	EXPECT_EXIT(
		{
			AB_ASSERT(true);
			std::cerr << "Success" << std::endl;
			exit(0);
		},
		testing::ExitedWithCode(0), "Success");
}
