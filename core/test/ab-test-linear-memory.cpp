#include <Ab/Config.hpp>
#include <Ab/LinearMemory.hpp>
#include <gtest/gtest.h>

namespace Ab::Test {

TEST(LinearMemoryTest, init_and_kill) { LinearMemory m; }

TEST(LinearMemoryTest, grow_three_times) {
	LinearMemory m;
	for (std::size_t i = 0; i < 3; i++) {
		m.grow();
	}
}

}  // namespace Ab::Test
