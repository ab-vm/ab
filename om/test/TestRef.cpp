#include <Om/Ref.hpp>
#include <gtest/gtest.h>

using namespace Om;

TEST(Ref, Constructor) {
	Ref<void> ref;
	EXPECT_EQ(ref, nullptr);

	Ref<const int> r2;
	EXPECT_EQ(ref, nullptr);
}

TEST(Ref, Dereference) {
	int i = 1;
	Ref<const int> ref{&i};
	EXPECT_EQ(*ref, i);
}

struct MyStruct {
	int i_;
};

TEST(Ref, Arrow) {
	MyStruct s{1};
	Ref<const MyStruct> ref{&s};
	EXPECT_EQ(ref->i_, s.i_);
}

TEST(Ref, Raw) {
	MyStruct s{1};
	Ref<const MyStruct> ref{&s};
	EXPECT_EQ(ref.raw(), &s);
}