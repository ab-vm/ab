#include <Ab/Edn/Lexer.hpp>
#include <Ab/Edn/Printing.hpp>
#include <Ab/Edn/DebugPrinting.hpp>
#include <Ab/Test/Env.hpp>

#include <vector>
#include <optional>
#include <iostream>
#include <fstream>

#include <gtest/gtest.h>
#include <fmt/core.h>

namespace Ab::Edn::Test {

using namespace ::Ab::Test;

std::vector<Token> get_tokens(Lexer& lexer) {
	std::vector<Token> tokens;
	while(lexer.more()) {
		tokens.push_back(lexer.next());
	}
	return tokens;
}

std::vector<Token> lex(Parse::LineInfo& line_info, std::string_view str) {
	Lexer lexer(line_info, str.begin(), str.end());
	return get_tokens(lexer);
}

std::vector<Token> lex(std::string_view str) {
	Parse::LineInfo line_info;
	return lex(line_info, str);
}

std::vector<Token> lex_file(Parse::LineInfo& line_info, const char* filename) {
	return lex(line_info, read_file(filename));
}

std::vector<Token> lex_file(const char* filename) {
	Parse::LineInfo line_info;
	return lex_file(line_info, filename);
}

/// Given an input, assert that the tokens returned by the lexer match the 
/// tokens passed in. A helper for building tokenizer test.
void expect_tokens(std::string_view input, const std::vector<Token>& tokens) {
	Parse::LineInfo line_info;
	Lexer lexer(line_info, input.begin(), input.end());
	for (const auto& token : tokens) {
		EXPECT_EQ(lexer.next(), token);
	}
}

void expect_tokens_in_source_file(const char* filename, const std::vector<Token>& tokens) {
	expect_tokens(env->read_source_file(filename).data(), tokens);
}

TEST(lexer, empty_string) {
	expect_tokens("", {
		tok_end(src_range(0, 0))
	});
}

TEST(lexer, just_a_space) {
	expect_tokens(" ", {
		tok_end(src_range(1, 1))
	});
}

TEST(lexer, just_4_spaces) {
	expect_tokens("    ", {
		tok_end(src_range(4, 4))
	});
}

TEST(lexer, nil) {
	expect_tokens("nil", {
		tok_nil(src_range(0, 3)),
		tok_end(src_range(3, 3))
	});
}

TEST(lexer, bool_true) {
	expect_tokens("true", {
		tok_bool(src_range(0, 4), true),
		tok_end( src_range(4, 4))
	});
}

TEST(lexer, bool_false) {
	expect_tokens("false", {
		tok_bool(src_range(0, 5), false),
		tok_end( src_range(5, 5))
	});
}

TEST(lexer, lparen) {
	expect_tokens("(", {
		{TokenKind::LPAREN, src_range(0, 1)},
	});
}

TEST(lexer, lparen_then_end) {
	expect_tokens("(", {
		{TokenKind::LPAREN, src_range(0, 1)},
		tok_end(src_range(1, 1))
	});
}

TEST(lexer, rparen) {
	expect_tokens(")", {
		{TokenKind::RPAREN, src_range(0, 1)},
		tok_end(src_range(1, 1))
	});
}

TEST(lexer, just_a) {
	expect_tokens("A", {
		tok_symbol(src_range(0, 1)),
		tok_end(src_range(1, 1))
	});
}

TEST(lexer, just_ab) {
	auto test = "AB";
	expect_tokens(test, {
		tok_symbol(src_range(0, 2)),
		tok_end(src_range(2, 2))
	});
}

TEST(lexer, ab_paren) {
	auto test = "AB)";
	expect_tokens(test, {
		tok_symbol(src_range(0, 2)),
		{TokenKind::RPAREN, src_range(2, 3)},
		tok_end(src_range(3, 3))
	});
}

TEST(lexer, ab_space_paren) {
	auto test = "AB )";
	expect_tokens(test, {
		tok_symbol(src_range(0, 2)),
		{TokenKind::RPAREN, src_range(3, 4)},
		tok_end(src_range(4, 4))
	});
}

TEST(lexer, just_newline) {
	std::string_view in("\n");
	LineInfo line_info;
	Lexer lexer(line_info, in.begin(), in.end());

	auto token = lexer.next();
	EXPECT_EQ(token.kind(), TokenKind::END);
	EXPECT_EQ(token.src_range(), src_range(1, 1));

	ASSERT_EQ(line_info.breaks().size(), 2);
	EXPECT_EQ(line_info.breaks()[0], 0);
	EXPECT_EQ(line_info.breaks()[1], 1);
}

TEST(lexer, token_after_newline) {
	std::string_view in = "\nABC";
	LineInfo line_info;
	Lexer lexer(line_info, in.begin(), in.end());

	auto tokens = get_tokens(lexer);
	ASSERT_EQ(tokens.size(), 1);

	const auto& token = tokens[0];
	ASSERT_EQ(token.kind(), (TokenKind::SYMBOL));
	EXPECT_EQ(token.strv(in), "ABC");
	EXPECT_EQ(token.src_begin(), src_pos(1));
	EXPECT_EQ(token.src_end(), src_pos(4));
	EXPECT_EQ(token.location(line_info), Location(1, 0));

	ASSERT_EQ(line_info.breaks().size(), 2);
	EXPECT_EQ(line_info.breaks()[0], 0);
	EXPECT_EQ(line_info.breaks()[1], 1);
}

TEST(lexer, token_after_some_whitespace) {
	std::string_view in = "\n\n ABC";
	LineInfo line_info;
	Lexer lexer(line_info, in.begin(), in.end());

	auto tokens = get_tokens(lexer);
	ASSERT_EQ(tokens.size(), 1);

	const auto& token = tokens[0];
	ASSERT_EQ(token.kind(), (TokenKind::SYMBOL));
	EXPECT_EQ(token.strv(in), "ABC");
	EXPECT_EQ(token.location(line_info), Location(2, 1));

	ASSERT_EQ(line_info.breaks().size(), 3);
	EXPECT_EQ(line_info.breaks()[0], 0);
	EXPECT_EQ(line_info.breaks()[1], 1);
	EXPECT_EQ(line_info.breaks()[2], 2);
}

TEST(lexer, empty_comment) {
	expect_tokens(";", {tok_end(src_range(1, 1))});
}

TEST(lexer, simple_comment) {
	expect_tokens("; 123456789", {tok_end(src_range(11, 11))});
}

TEST(lexer, things) {
	expect_tokens(
		"(\n"
		"; 1234\n"
		")\n",
		{
			{TokenKind::LPAREN, src_range( 0,  1)},
			{TokenKind::RPAREN, src_range( 9, 10)},
			tok_end(            src_range(11, 11))
		}
	);
}

TEST(lexer, module_symbol) {
	auto test = "module";
	expect_tokens(test, {
		tok_symbol(src_range(0, 6)),
		tok_end(   src_range(6, 6))
	});
}

TEST(lexer, func_symbol) {
	auto test = "func";
	expect_tokens(test, {
		tok_symbol(src_range(0, 4)),
		tok_end(   src_range(4, 4))
	});
}

TEST(lexer, int_1) {
	auto test = "1";
	expect_tokens(test, {
		tok_integer(src_range(0, 1), 1),
		tok_end(    src_range(1, 1))
	});
}

TEST(lexer, int_neg_1) {
	auto test = "-1";
	expect_tokens(test, {
		tok_integer(src_range(0, 2), -1),
		tok_end(src_range(2, 2))
	});
}

TEST(lexer, int_pos_1) {
	auto test = "+1";
	expect_tokens(test, {
		tok_integer(src_range(0, 2), 1),
		tok_end(src_range(2, 2))
	});
}

TEST(lexer, some_ints) {
	auto test = "+1 -1 2 3 123456789";
	expect_tokens(test, {
		tok_integer(src_range( 0,  2),         1),
		tok_integer(src_range( 3,  5),        -1),
		tok_integer(src_range( 6,  7),         2),
		tok_integer(src_range( 8,  9),         3),
		tok_integer(src_range(10, 19), 123456789),
		tok_end(    src_range(19, 19))
	});
}

struct SymbolTestCase {
	std::string_view str;
};

TEST(lexer, symbol) {
	std::string_view test_cases[] = {
		":test", ":also-a-test"
	};

	for (const auto& test : test_cases) {
		LineInfo line_info;
		Lexer lexer(line_info, test.begin(), test.end());
		auto token = lexer.next();
		EXPECT_EQ(token.kind(), TokenKind::KEYWORD);
		EXPECT_EQ(token.strv(test), test);
	}
}

struct RealTestCase {
	std::string_view str;
	double val;
};

TEST(lexer, reals) {
	const RealTestCase test_cases[] = {
		{"0.0", 0},
		{"+0.0", 0},
		{"-0.0", -0},
		{"+0.1", 0.1},
		{"-1.1", -1.1},
		{"1E0", 1},
		{"1E1", 10},
		{"2e3", 2000},
		{"1E+1", 10},
		{"1E-1", 0.1},
		{"1234e+3", 1234000.0},
		{"1234e-3", 1.234},
		{"12.34e-2", 0.1234}
	};

	for (const auto& test : test_cases) {
		EXPECT_EQ(test.val, parse_real(test.str.begin(), test.str.end())) << "test_case=" << test.str;
	}

	for (const auto& test : test_cases) {
		LineInfo line_info;
		Lexer lexer(line_info, test.str.begin(), test.str.end());
		auto token = lexer.next();
		EXPECT_EQ(token.kind(), TokenKind::REAL);
		EXPECT_EQ(token.data().as_real, test.val);
	}
}

TEST(lexer, empty_module) {
	// expect_tokens_in_source_file("abt/test/modules/empty-module.abt", {"(", "module", ")"});
}

}  // namespace Ab::Abt::Test
