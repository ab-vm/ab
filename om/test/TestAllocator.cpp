#include <Om/Config.hpp>
#include <Om/Allocator.hpp>
#include <Om/NativeCell.hpp>
#include <gtest/gtest.h>

namespace Om {
namespace Test {

#if OM_DISABLE

struct IntCell : public NativeCell {
public:
	IntCell() : IntCell(0) {
	}
	IntCell(int x) : NativeCell{sizeof(IntCell)}, value{x} {
	}
	int value;
};

TEST(Allocator, base) {
	Process::init();
	System sys{SystemConfig{}};
	Context cx{sys};
	// Om::Allocator<Cell> allocator{cx};
	// EXPECT_TRUE(result);
	// StackRootRef<IntCell> root{acx, result()};
	// EXPECT_EQ(root->value, 1);
	// EXPECT_EQ(root->size(), sizeof(IntCell));
	Process::kill();
}

TEST(Allocator, allocateStruct) {
	Maybe<StructCell<MyStruct>*> m = allocateStruct<MyStruct>(42);
	EXPECT_TRUE(m);
	EXPECT_EQ(*m->x, 42);
	EXPECT_EQ(**m.x, 42);
}

TEST(Allocator, allocateArray) {
	Maybe<ArrayCell<MyStruct>*> m = allocateArray<MyStruct>(42);
	*m[42]                        = 15;
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

#endif  // OM_DISABLE

}  // namespace Test
}  // namespace Om
