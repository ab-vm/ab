#include <Ab/Config.hpp>
#include <Ab/LinearMemory.hpp>
#include <Ab/Test/BasicTest.hpp>
#include <gtest/gtest.h>

namespace Ab {
namespace Test {

class LinearMemoryTest : public BasicTest {};

TEST_F(LinearMemoryTest, initAndKill) {
	LinearMemory m{};
}

TEST_F(LinearMemoryTest, grow3) {
	LinearMemory m{};
	for (std::size_t i = 0; i < 3; i++) {
		m.grow();
	}
}

}  // namespace Test
}  // namespace Ab
