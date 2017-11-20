#include <Ab/Box.hpp>
#include <gtest/gtest.h>

using namespace Ab;

class Destructor {};

TEST(TestBox, defaultConstructor) {
	Box<int> box{IN_PLACE};
	box() = 1;
	EXPECT_EQ(box(), 1);
}

TEST(TextBox, copyConstructor) {
	Box<int> b{5};
	EXPECT_EQ(b(), 5);
}

struct MyStruct {
	int x;
};

TEST(TestBox, Constructor) {
	Box<MyStruct> box{MyStruct{2}};
	EXPECT_EQ(box().x, 2);
}

TEST(TestBox, BoxBox) {
	Box<int> box1{5};
	EXPECT_EQ(box1(), 5);

	Box<Box<int>> box2{box1};
	EXPECT_EQ(box2()(), 5);

	Box<Box<Box<int>>> box3{box2};
	EXPECT_EQ(box3()()(), 5);
}
