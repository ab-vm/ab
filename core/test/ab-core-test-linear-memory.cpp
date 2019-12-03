#include <Ab/Config.hpp>
#include <Ab/LinearMemory.hpp>
#include <gtest/gtest.h>

namespace Ab::Test {

TEST(LinearMemoryTest, InitAndKill) { LinearMemory m; }

TEST(LinearMemoryTest, GrowThreeTimes) {
	LinearMemoryConfig cfg;
	cfg.page_count_min = 0;
	cfg.page_count_max = 3;

	LinearMemory m(cfg);
	for (std::size_t i = 0; i < 3; i++) {
		m.grow();
	}
}

TEST(LinearMemoryTest, AssignToMemory) {
	LinearMemoryConfig cfg;
	cfg.page_count_min = 1;
	cfg.page_count_max = 1;

	LinearMemory m(cfg);

	auto ptr = to_ptr<int>(m.address());
	EXPECT_NE(ptr, nullptr);
	EXPECT_EQ(*ptr, 0);
	*ptr = 123;
	EXPECT_EQ(*ptr, 123);
}

}  // namespace Ab::Test
