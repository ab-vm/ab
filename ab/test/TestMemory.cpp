#include <Ab/Config.hpp>
#include <Ab/Memory.hpp>
#include <Ab/Test/BasicTest.hpp>
#include <gtest/gtest.h>

namespace Ab {
namespace Test {

class TestMemory : public BasicTest {};

TEST_F(TestMemory, initAndKill) {
	Memory m;
	ASSERT_EQ(m.init(Memory::defaultConfig()), MemoryError::SUCCESS);
	m.kill();
}

TEST_F(TestMemory, grow) {
	Memory m;
	m.init(Memory::defaultConfig());
	for (std::size_t i = 0; i < 3; i++) {
		EXPECT_EQ(m.grow(), MemoryError::SUCCESS);
	}
	m.kill();
}

}  // namespace Test
}  // namespace Ab
