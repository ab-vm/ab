#include <Pith/StringSpan.hpp>
#include <gtest/gtest.h>

using namespace Pith;

TEST(TestStringSpan, equality) {
	// vs stringspan
	EXPECT_EQ(StringSpan{"abc"}, StringSpan{"abc"});
	EXPECT_NE(StringSpan{"abc"}, StringSpan{"def"});
	EXPECT_NE(StringSpan{"abc"}, StringSpan{"abcd"});
	EXPECT_NE(StringSpan{"abcd"}, StringSpan{"abc"});
	EXPECT_EQ((StringSpan{"abcd", 3}), StringSpan{"abc"});

	// vs char
	EXPECT_EQ(StringSpan{"abc"}, "abc");
	EXPECT_NE(StringSpan{"abc"}, "def");
	EXPECT_NE(StringSpan{"abc"}, "abcd");
}
