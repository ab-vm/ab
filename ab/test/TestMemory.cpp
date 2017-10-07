#include <Ab/Config.hpp>
#include <Ab/Memory.hpp>
#include <Ab/Test/BasicTest.hpp>
#include <gtest/gtest.h>

namespace Ab {
namespace Test {

class TestMemory : public BasicTest {};

TEST_F(TestMemory, initAndKill) {
	Memory m{};
}

TEST_F(TestMemory, grow) {
	Memory m{};
	for (std::size_t i = 0; i < 3; i++) {
		m.grow();
	}
}

}  // namespace Test
}  // namespace Ab
