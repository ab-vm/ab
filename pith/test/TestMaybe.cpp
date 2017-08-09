#include <Pith/Maybe.hpp>
#include <gtest/gtest.h>

using namespace Pith;

TEST(Maybe, empty) {
	EXPECT_FALSE(Maybe<int>());
	EXPECT_FALSE(Maybe<int>(nothing));
}

TEST(Maybe, value) {
	int i = 1;
	Maybe<int> m{i};
	EXPECT_TRUE(m);
	EXPECT_TRUE(Maybe<int>(1));
	EXPECT_TRUE(Maybe<int>(inPlace, 1));
	EXPECT_TRUE(just(1));
}

struct MyDefaultConstructibleType {
	MyDefaultConstructibleType() : x_{1} {
	}
	int x_;
};

TEST(MAybe, defaultConstructor) {
	Maybe<MyDefaultConstructibleType> m(inPlace);
}

struct HasCons {
	int x_;
	HasCons(int x) : x_(x + 1) {
	}
};

template <typename T>
void fail(T x) {
	ADD_FAILURE();
};

TEST(Maybe, application) {
	auto&& increment = [](int x) -> int { return x + 1; };
	Maybe<int> m1{1};
	EXPECT_EQ(m1.map(increment).just(), 2);
	EXPECT_EQ(just(1).map(increment).just(), 2);

	Maybe<int>{nothing}.apply(fail<int>);  // not called

	Maybe<double> m3 = Maybe<int>{1}.map([](int&& i) -> double {
		EXPECT_EQ(i, 1);
		return 2.0;
	});
	EXPECT_EQ(*m3, 2.0);
}

TEST(Maybe, PointerValue) {
	EXPECT_FALSE(Maybe<void*>{nullptr});
	EXPECT_FALSE(Maybe<void*>{nothing});
	EXPECT_TRUE(Maybe<void*>{this});
	Maybe<void*>{}.apply(fail<void*>);  // fail <void*> not called
}

TEST(Maybe, MaybeMaybe) {
	// size overhead - none for nested maybe's
	EXPECT_EQ(sizeof(Maybe<int>), sizeof(Maybe<Maybe<int>>));
	EXPECT_EQ(sizeof(void*), sizeof(Maybe<Maybe<void*>>));
	EXPECT_EQ(sizeof(Maybe<void*>), sizeof(Maybe<Maybe<void*>>));

	// non-nullable
	EXPECT_FALSE(Maybe<Maybe<int>>{nothing});
	EXPECT_TRUE(Maybe<Maybe<int>>{1});

	// nullable
	EXPECT_FALSE(Maybe<Maybe<void*>>{nullptr});
	EXPECT_FALSE(Maybe<Maybe<void*>>{nothing});
	EXPECT_TRUE(Maybe<Maybe<void*>>{this});
}

#if 0

TEST(Maybe, foreach) {
	for (auto&& i : m2) {
		EXPECT_EQ(i, 1);
	}
	Maybe<int> m1{555};
	for (auto&& x : m1) {
		EXPECT_EQ(x, 555);
	}
	
	Maybe<int> m2{nothing};
	for (auto& x: m2) {
		FAIL() << "this code is unreachable";
	}}
}
#endif  // 0

TEST(Maybe, HasCons) {
	Maybe<HasCons> ma(nothing);
	Maybe<HasCons> mb;
	Maybe<HasCons> mc(inPlace, 42);
	Maybe<HasCons> md = HasCons(42);
	// EXPECT_EQ(*Maybe<HasCons>(69).x, 69);
}

TEST(Maybe, dereferenceAssignSimple) {
	Maybe<double> md(2);
	Maybe<int> m(1);
	*m = 2;
	EXPECT_EQ(*m, 2);
	EXPECT_TRUE(m);
}

#if 0
// Arrays-as-values are not yet supported by maybe
TEST(Maybe, arrayValue) {
	int i[2] = {1, 2};
	Maybe<int*> mp(i);

	// nothing
	Maybe<int[2]> m1 = nothing;
	EXPECT_FALSE(m1);

	// just
	Maybe<int[2]> m2(inPlace) = just({0, 1});
	EXPECT_TRUE(m2);
	EXPECT_EQ(*m2[0], 0);
	EXPECT_EQ(*m2[1], 1);

	EXPECT_EQ(sizeof(m1), sizeof(m2));
}
#endif  // 0

TEST(Maybe, pointerValue) {
	int i = 1;
	int* p = &i;
	Maybe<int*> m(p);
	EXPECT_EQ(*m, p);
	EXPECT_EQ(**m, i);
	EXPECT_TRUE(Maybe<int*>(inPlace, (int*)1));
	EXPECT_TRUE(m);
	EXPECT_EQ(sizeof(m), sizeof(p)) << "Pointers should have no storage overhead.";
}

TEST(Maybe, emptyPointerValue) {
	EXPECT_FALSE(Maybe<int*>(nullptr));
	EXPECT_FALSE(Maybe<int*>(nothing));
	EXPECT_FALSE(Maybe<int*>());
	EXPECT_FALSE(Maybe<int*>(inPlace, nullptr));
}

TEST(Maybe, dereferenceAssignPointer) {
	int a = 1;
	int b = 2;
	Maybe<int*> m(&a);
	*m = &b;
	EXPECT_EQ(*m, &b);
	EXPECT_TRUE(m);
}

#if 0
/// references disallowed
TEST(Maybe, referenceValue) {
	int i = 1;
	int& x = i;
	Maybe<int&> ma(i);
	Maybe<int&> mb(x);
	EXPECT_TRUE(ma);
	EXPECT_TRUE(mb);
	EXPECT_EQ(*ma, *mb);
	i = 2;
	EXPECT_EQ(*ma, i);
	EXPECT_EQ(*mb, i);
}
#endif  // 0