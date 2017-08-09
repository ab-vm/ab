#include <Pith/Box.hpp>
#include <gtest/gtest.h>

using namespace Pith;

TEST(TestBox, defaultConstructor) {
	Box<int> i{2};
	i = 1;
	EXPECT_EQ(i(), 1);
}

struct Int {
	int x;
};

TEST(TestBox, Constructor) {
	Box<Int> b{Int{1}};
}
