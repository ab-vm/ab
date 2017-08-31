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

}  // namespace Test
}  // namespace Ab