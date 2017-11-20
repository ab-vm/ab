#include <Ab/Edn/Parser.hpp>
#include <Ab/Edn/Printing.hpp>
#include <Ab/Edn/Ir.hpp>
#include <Ab/Test/Env.hpp>

#include <gtest/gtest.h>

namespace Ab::Edn::Test {

using namespace ::Ab::Test;

void check_log(const ErrorLog& log) {
	if (log.empty()) {
		SUCCEED();
		return;
	}

	for (const auto& e : log.errors()) {
		fmt::print(stderr, "+{}: error: {}\n", e.pos.begin().get(), e.msg);
	}

	FAIL();
}

/// Parse the input, and assert that the parsed nodes match up.
///
void expect_nodes(std::string_view input, const std::vector<Ptr<Node>>& nodes) {
	Document doc = parse(input.begin(), input.end());
	EXPECT_TRUE(doc.error_log.empty());
	ASSERT_EQ(doc.nodes.size(), nodes.size());
	for (std::size_t i = 0; i < nodes.size(); ++i) {
		const auto& a = *doc.nodes[i];
		const auto& b = *doc.nodes[i];
		EXPECT_TRUE(node_eq(a, b)); // << "a=" << a << " b=" << b;
	}
}

TEST(parser, empty_file) {
	expect_nodes("", {});
}

TEST(parser, just_nil) {
	expect_nodes("nil", {node_nil()});
	expect_nodes("nil nil", {node_nil(), node_nil()});
}

TEST(parser, some_bools) {
	expect_nodes("true false", {
		node_bool(true),
		node_bool(false)
	});
}

TEST(parser, some_ints) {
	expect_nodes("0 1 22 333 4444 55555 666666", {
		node_integer(0),
		node_integer(1),
		node_integer(22),
		node_integer(333),
		node_integer(4444),
		node_integer(55555),
		node_integer(666666)
	});
}

TEST(parser, some_reals) {

}

TEST(parser, a_list) {
	expect_nodes("(1 2 3)", {
		node_list({
			node_integer(1),
			node_integer(2),
			node_integer(3)
		})
	});
}

TEST(parser, a_vector) {
	expect_nodes("[1 2 3]", {
		node_vector({
			node_integer(1),
			node_integer(2),
			node_integer(3)
		})
	});
}

TEST(parser, a_map) {
	expect_nodes("{1 2, 3 4}", {
		node_map({
			{node_integer(1), node_integer(2)},
			{node_integer(3), node_integer(4)}
		})
	});

	expect_nodes("{[1 2] 0, (1 2) 1}", {
		node_map({
			{
				node_vector({
					node_integer(1),
					node_integer(2)
				}),
				node_integer(0)
			},
			{
				node_list({
					node_integer(1),
					node_integer(2)
				}),
				node_integer(1)
			}
		})
	});
}

TEST(parser, a_string) {
	expect_nodes("\"abc\"", {node_string("abc")});
}

TEST(parse, list_of_symbols) {
	expect_nodes("(a b c)", {
		node_list({
			node_symbol("a"),
			node_symbol("b"),
			node_symbol("c")
		})
	});
}

TEST(parser, a_keyword) {
	expect_nodes(":my-test", {node_keyword(":my-test")});
}

TEST(parser, another_map) {

	std::string_view input = "{:test [12 34]}";
	// Document doc = parse(input.begin(), input.end());
	// EXPECT_TRUE(doc.error_log.empty());
	
	// Ptr<Map> map = node_cast<Map>(doc.nodes[0]);
	// Ptr<Node> value = map->lookup(node_keyword("test"));

	expect_nodes(input, {
		node_map({
			{
				node_keyword(":test"),
				node_vector({node_integer(12), node_integer(34)})
			}
		})
	});
}

} // namespace Ab::Abt::Test
