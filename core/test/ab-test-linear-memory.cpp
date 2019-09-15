#include <Ab/Config.hpp>
#include <Ab/LinearMemory.hpp>
#include <Ab/Test/BasicTest.hpp>
#include <gtest/gtest.h>

namespace Ab::Test {

class LinearMemoryTest : public BasicTest {};

TEST_F(LinearMemoryTest, init_and_kill) { LinearMemory m; }

TEST_F(LinearMemoryTest, grow_three_times) {
	LinearMemory m;
	for (std::size_t i = 0; i < 3; i++) {
		m.grow();
	}
}

}  // namespace Ab::Test
