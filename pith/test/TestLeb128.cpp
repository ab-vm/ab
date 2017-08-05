#include <Pith/Leb128.hpp>
#include <cstdint>
#include <gtest/gtest.h>

using namespace Pith;

#ifdef PITH_DISABLE

TEST(TestLeb128, int) {
	ASSERT_EQ(Leb128<std::uint8_t>(8), 8);
}

#endif // PITH_DISABLE
