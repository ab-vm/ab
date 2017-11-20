
#include <Ab/Vector.hpp>

#include <gtest/gtest.h>

using namespace Ab;

TEST(TestSizeP2, Pow2) {
	EXPECT_EQ(0 % SIZEP2_2, 0);
	EXPECT_EQ(1 % SIZEP2_2, 1);
	EXPECT_EQ(2 % SIZEP2_2, 0);

	EXPECT_EQ(0 % SIZEP2_4, 0);
	EXPECT_EQ(1 % SIZEP2_4, 1);
	EXPECT_EQ(2 % SIZEP2_4, 2);
	EXPECT_EQ(3 % SIZEP2_4, 3);
	EXPECT_EQ(4 % SIZEP2_4, 0);
}

struct RcInt : RefCounted {
	constexpr RcInt(int v) : value(v) {}
	int value;
};

TEST(TestRc, CreateDeadReferent) {
	RcInt* i = new RcInt(1);
	EXPECT_TRUE(i->refcounter().dead());
	EXPECT_EQ(i->refcounter().value(), 0);
	EXPECT_FALSE(i->refcounter().live());
	delete i;
}

TEST(TestRc, CreateRc) {
	auto x = mkrc<RcInt>(1);
	EXPECT_TRUE(x->refcounter().live());
	EXPECT_EQ(x->refcounter().value(), 1);
	EXPECT_FALSE(x->refcounter().dead());
}

TEST(TestRc, CopyRc) {
	auto x = mkrc<RcInt>(42);
	EXPECT_EQ(x->refcounter().value(), 1);

	Rc<RcInt> y = x;
	EXPECT_EQ(x->refcounter().value(), 2);

	x.clear();
	EXPECT_EQ(y->refcounter().value(), 1);
}

struct DtorCalled : public RefCounted {

	DtorCalled(bool& called) : called(called) {
		called = false;
	}

	~DtorCalled() { called = true; }

	bool& called;
};

TEST(TestRc, DtorCalledAfterRcCleared) {
	bool called = false;
	auto x = mkrc<DtorCalled>(called);
	x.clear();
	EXPECT_TRUE(called);
}

TEST(TestRc, DtorCalledAfterAllRcCleared) {
	bool called = false;
	auto x = mkrc<DtorCalled>(called);
	auto y = x;
	x.clear();
	EXPECT_FALSE(called);
	y.clear();
	EXPECT_TRUE(called);
}

TEST(TestVector, createOne) {
	Vec<int> vec;
}

TEST(TestVector, consThreeCopyConstruct) {
	Vec<int> v0;
	auto v1 = v0.cons(1);
	auto v2 = v1.cons(2);
	auto v3 = v2.cons(3);
	EXPECT_EQ(v3[0], 1);
	EXPECT_EQ(v3[1], 2);
	EXPECT_EQ(v3[2], 3);
}

void printInt(int x) {
	fprintf(stderr, "%d ", x);
}

TEST(TestVector, cons32) {
	Vec<int> vec;
	for (auto i = 0; i < 9; ++i) {
		// fprintf(stderr, "**** step %d\n", i);
		// fprintf(stderr, "capacity %d\n", vec.capacity());
		vec = vec.cons(i);
		EXPECT_EQ(vec.size(), i + 1);
		EXPECT_EQ(vec[i], i);
		vec.dump();
		fprintf(stderr, "\n");
	}
}

TEST(TestVector, cons64) {
	Vec<int> vec;
	for (auto i = 0; i < 64; ++i) {
		vec = vec.cons(i);
		EXPECT_EQ(vec.size(), i + 1);
		EXPECT_EQ(vec[i], i);
		vec.foreach(printInt);
		fprintf(stderr, "\n");
	}
}

TEST(TestVector, LoopOver32) {
	Vec<int> vec;
	for (auto i = 0; i < 64; ++i) {
		vec = vec.cons(i);
		EXPECT_EQ(vec.size(), i + 1);
		EXPECT_EQ(vec[i], i);
		// vec.foreach(printInt);
		fprintf(stderr, "\n");
	}

	for (int x : vec) {
		fprintf(stderr, "%d\n", x);
	}
}
