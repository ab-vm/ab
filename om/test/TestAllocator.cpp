#include <Om/Allocator.hpp>
#include <gtest/gtest.h>

using namespace Om;

struct MyStruct {
	MyStruct(int x) : x{x} {
	}
	int x;
};

TEST(Allocator, base) {
	Om::Allocator allocator;
}

#if OM_DISABLE

TEST(Allocator, allocateStruct) {
	Maybe<StructCell<MyStruct>*> m = allocateStruct<MyStruct>(42);
	EXPECT_TRUE(m);
	EXPECT_EQ(*m->x, 42);
	EXPECT_EQ(**m.x, 42);
}

TEST(Allocator, allocateArray) {
	Maybe<ArrayCell<MyStruct>*> m = allocateArray<MyStruct>(42);
	*m[42] = 15;
	EXPECT_TRUE(m);
	EXPECT_EQ(*m[42].x, 15);
}

TEST(Allocator, structAllocation) {
	auto allocation = allocate<std::tuple<int, int>>(1, 2);
	EXPECT_TRUE(allocation);
	auto t = *allocation;
	EXPECT_EQ(t[0], 1);
	EXPECT_EQ(t[]);
};

TEST(Allocator, allocate) {
	Runtime runtime;
	Error e;

	e = runtime.init();
	ASSERT_FALSE(e);

	Context c(runtime);
	Maybe<Object*> m = c.allocator().allocate();
	EXPECT_TRUE(m);

	e = runtime.kill();
	EXPECT_FALSE(e);
}
#endif  // OM_DISABLE