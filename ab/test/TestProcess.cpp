#include "Ab/Process.hpp"
#include <gtest/gtest.h>

TEST(TestProcess, startAndStop) {
	for (int i = 0; i < 3; i++) {
		EXPECT_EQ(Ab::Process::init(), Ab::Error::SUCCESS);
		EXPECT_EQ(Ab::Process::kill(), Ab::Error::SUCCESS);
	}
}
