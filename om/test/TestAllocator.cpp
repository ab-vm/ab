#include <Om/Config.hpp>
#include <Om/Allocator.hpp>
#include <Om/NativeCell.hpp>
#include <gtest/gtest.h>

#if 0

namespace Om {
namespace Test {

struct IntCell : public NativeCell {
public:
	IntCell() : IntCell(0) {
	}
	IntCell(int x) : NativeCell{sizeof(IntCell)}, value{x} {
	}
	int value;
};

TEST(Allocator, base) {
	System sys{};
	sys.init(System::DEFAULT_CONFIG);
	Context cx{sys};
	cx.init();
	{
		ActiveContext acx{cx};
		Om::Allocator allocator;
		auto result = allocator.allocateNative<GcSafe::YES, IntCell>(acx, 1);
		EXPECT_TRUE(result);
		StackRootRef<IntCell> root{acx, result()};
		EXPECT_EQ(root->value, 1);
		EXPECT_EQ(root->size(), sizeof(IntCell));
	}
	cx.kill();
	sys.kill();
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
	-0

		Contexjuuk,
		, , , , , , , , t c(runtime);
	Maybe<Object*> m = c.allocator().allocate();
	EXPECT_TRUE(m);

	e = runtime.kill();
	EXPECT_FALSE(e);
}

#endif  // OM_DISABLE

}  // namespace Test
}  // namespace Om

#endif  // 0
