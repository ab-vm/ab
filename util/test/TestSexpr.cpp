#include <Ab/Sexpr.hpp>
#include <gtest/gtest.h>

namespace Ab::Test {

TEST(TestSexpr, Empty) {
	Sexpr::Formatter fmt;
	fmt << "";
	EXPECT_EQ(fmt.get(), "");
}

TEST(TestSexpr, EmptySexpr) {
	Sexpr::Formatter fmt;
	fmt << Sexpr::START << Sexpr::END;
	EXPECT_EQ(fmt.get(), "()");
}

TEST(TestSexpr, OneThing) {
	Sexpr::Formatter fmt;
	fmt << Sexpr::START
		<< "test"
		<< Sexpr::END;
	EXPECT_EQ(fmt.get(), "(test)");
}

TEST(TestSexpr, TwoThings) {
	Sexpr::Formatter fmt;
	fmt << Sexpr::START
		<< "a"
		<< "b"
		<< Sexpr::END;
	EXPECT_EQ(fmt.get(), "(a b)");
}

TEST(TestSexpr, NestedSexpr) {
	Sexpr::Formatter fmt;
	fmt << Sexpr::START
		<< "a" << "b"
		<< Sexpr::FRESH << Sexpr::START << "c" << Sexpr::END
		<< Sexpr::END;

	EXPECT_EQ(fmt.get(), "(a b\n  (c))");
}

}  // namespace Ab::Test
