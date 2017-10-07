#include <Om/Value.hpp>
#include <gtest/gtest.h>

#if 0

namespace Om {
namespace Test {

TEST(Value, constructor) {
	[[gnu::unused]] Value x1 = 1;
	[[gnu::unused]] Value x2 = 1.0;
	[[gnu::unused]] Value x3 = nullptr;
}

TEST(Value, to) {
	Value x = 1;
	EXPECT_EQ(1, Value(1).to<int>());
}

} // namespace Test
} // namespace Om

#endif  // 0