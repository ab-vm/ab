#include "Ab/Process.hpp"
#include <gtest/gtest.h>

namespace Ab {
namespace Test {

TEST(TestProcess, startAndStop) {
	for (int i = 0; i < 3; i++) {
		EXPECT_EQ(Process::init(), Error::SUCCESS);
		EXPECT_EQ(Process::kill(), Error::SUCCESS);
	}
}

}  // namespace Test
}  // namespace Ab
