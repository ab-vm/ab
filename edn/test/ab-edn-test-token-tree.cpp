#if 0

#include <gtest/gtest.h>
#include <Ab/Abt/TokenTree.hpp>

namespace Ab::Abt::Test {

TEST(token_tree, no_input) {

}

TEST(token_tree, simple_group) {

	std::string_view in = "( TEST )";
	State state;

	TokenTree tree = lex_tree(state, in.data(), in.length());
	Group& root = tree.root();
	EXPECT_EQ(root.length(), 2);

	TTNode& group_node = root[0];
	EXPECT_EQ(group_node.kind(), TTKind::GROUP);

	Group& group = group_node.as_group();
	EXPECT_EQ(group.length(), 3);

	// left paren delimiter
	{
		TTNode& lparen_node = group[0];
		EXPECT_EQ(lparen_node.kind(), TTKind::TOKEN);
		
		Token& lparen_token = lparen_node.as_token();
		EXPECT_EQ(lparen_token.kind(), TokenKind::DEL);

		tok_delimiter& lparen_del = lparen_token.as_del();
		EXPECT_EQ(lparen_del.kind(), DelimiterKind::LPAREN);
		EXPECT_EQ(lparen_del.pos(), src_pos(0));
	}

	// TEST symbol
	{
		TTNode& test_node = group[1];
		EXPECT_EQ(test_node.kind(), TTKind::TOKEN);
		
		Token& test_token = test_node.as_token();
		EXPECT_EQ(test_token.kind(), TokenKind::SYM);

		TSym& test_sym = test_token.as_sym();
		EXPECT_EQ(test_sym.pos(), src_range(2, 6));
		EXPECT_EQ(test_sym.strv(in), "TEST");
	}

	/// right paren delimiter
	{
		TTNode& rparen_node = group[2];
		EXPECT_EQ(rparen_node.kind(), TTKind::TOKEN);
		
		Token& rparen_token = rparen_node.as_token();
		EXPECT_EQ(rparen_token.kind(), TokenKind::DEL);

		tok_delimiter& rparen_del = rparen_token.as_del();
		EXPECT_EQ(rparen_del.kind(), DelimiterKind::RPAREN);
		EXPECT_EQ(rparen_del.pos(), src_pos(7));
	}

	// end token
	{
		TTNode& end_node = root[1];
		EXPECT_EQ(end_node.kind(), TTKind::TOKEN);
		
		Token& end_token = end_node.as_token();
		EXPECT_EQ(end_token.kind(), TokenKind::END);

		TEnd& end = end_token.as_end();
		EXPECT_EQ(end.pos(), src_pos(8));
	}
}

} // namespace Ab::Abt::Test

#endif
