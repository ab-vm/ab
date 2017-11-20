#include <Ab/Edn/Ir.hpp>

#include <fmt/core.h>

#include <gtest/gtest.h>

namespace Ab::Edn::Test {

TEST(edn_symbol_table, not_interned) {
	SymbolTable table;
	EXPECT_FALSE(table.interned("test"));
}

TEST(edn_symbol_table, intern) {
	SymbolTable table;
	auto id = table.intern("test");
	EXPECT_TRUE(table.interned("test"));
	EXPECT_EQ(table.intern("test"), id);
	EXPECT_EQ(table.intern("test"), id);
	EXPECT_EQ(table.string(id), "test");
}

TEST(edn_nil, comparison) {
	auto nil = node_nil();
	EXPECT_EQ(*nil, *nil);
	EXPECT_FALSE(*nil != *nil);
	EXPECT_TRUE(node_eq(*nil, *nil));
}

TEST(edn_nil, reproducible_hash) {
	auto nil0 = node_nil();
	auto nil1 = node_nil();
	EXPECT_EQ(nil0->hash(), nil1->hash());
}

TEST(edn_integer, compare_with_self) {
	for (auto x : std::vector<std::intptr_t>{0, 1, 2, -1, 100}) {
		auto node = node_integer(x);
		EXPECT_EQ(node->value(), x);
		EXPECT_EQ(*node, *node);
		EXPECT_TRUE(node_eq(*node, *node));
	}
}

TEST(edn_real, hash) {
	Ptr<Real> real1 = node_real(1234);
	Ptr<Real> real2 = node_real(1234);
	EXPECT_EQ(node_hash(*real1), node_hash(*real2));
}

TEST(edn_vector, empty_compare_with_self) {
	auto v = node_vector();
	EXPECT_EQ(*v, *v);
}

TEST(edn_vector, insert_one) {
	auto v = node_vector();
	v->elements().push_back(node_integer(1));
	EXPECT_EQ(static_cast<Integer&>(*v->at(0)), Integer(1));
}

TEST(edn_vector, reproduce_hash) {
	auto v0 = node_vector();
	v0->elements().push_back(node_integer(42));
	v0->elements().push_back(node_integer(42));

	auto v1 = node_vector();
	v1->elements().push_back(node_integer(42));
	v1->elements().push_back(node_integer(42));

	EXPECT_EQ(v0->hash(), v1->hash());
}

TEST(edn_vector, compare_with_self) {
	auto v = node_vector();
	v->elements().push_back(node_integer(1));
	EXPECT_EQ(*v, *v);
}

TEST(edn_map, insert_1234_test) {
	auto m = construct<Map>();
	m->elements().insert({node_integer(1234), node_string("test")});
	EXPECT_EQ(as_string(*m->at(node_integer(1234))), String("test"));
}

TEST(edn_map, reproduce) {
	auto m1 = construct<Map>();
	m1->elements().insert({node_string("heyoo"), node_vector()});

	auto m2 = construct<Map>();
	m2->elements().insert({node_string("heyoo"), node_vector()});

	EXPECT_EQ(*m1, *m2);
	EXPECT_EQ(node_hash(*m1), node_hash(*m2));
}

TEST(edn_map, map_as_a_key) {
	auto m = construct<Map>();

	// using a map as a key
	auto key = construct<Map>();
	key->elements().insert({node_integer(1324), node_integer(5678)});

	// and a map as a val
	auto val = construct<Map>();
	val->elements().insert({node_integer(8765), node_integer(4321)});

	m->elements().insert({key, val});

	Ptr<Node> lookup = m->at(key);
	EXPECT_TRUE(node_eq(*lookup, *val));
}

// KNOWN BUG
TEST(edn_map, mutate_key_after_insertion) {
	auto m   = construct<Map>();
	auto key = node_integer(1234);
	auto val = node_integer(42);

	m->elements().insert({key, val});

	// modify the key object after insertion
	key->set_value(5678);

	// try to look up the integer(42) object via the new key-value, should fail.
	try {
		Ptr<Node> lookup = m->at(node_integer(5678));
		FAIL();
	} catch (...) {
		SUCCEED();
	}

	// try to look up the object via the original key, should also fail.
	try {
		Ptr<Node> lookup = m->at(node_integer(1234));
		FAIL();
	} catch (...) {
		SUCCEED();
	}
}

TEST(edn_symbol, intern) {
	std::vector<std::string> strings = {
		"", "test", "test2", "\1\2\3", "forty_five"
	};
	for (const auto& str : strings) {
		EXPECT_FALSE(globals.symbol_table.interned(str));
		Ptr<Symbol> sym1 = node_symbol(str);
		Ptr<Symbol> sym2 = node_symbol(str);
		EXPECT_EQ(*sym1, *sym2);
		EXPECT_EQ(sym1->id(), sym2->id());
		EXPECT_TRUE(node_eq(*sym1, *sym2));
		EXPECT_EQ(str, globals.symbol_table.string(sym1->id()));
		EXPECT_EQ(str, globals.symbol_table.string(sym2->id()));
	}
}

}  // namespace Ab::Edn::Test
