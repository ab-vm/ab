#if !defined(AB_WAST_LEXER_HPP_)
#define AB_WAST_LEXER_HPP_

#include <Ab/Wast/Alph.hpp>
#include <Ab/Wast/Location.hpp>
#include <fmt/format.h>
#include <queue>

template <>
struct fmt::formatter<Ab::Wast::Location> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		return ctx.begin();
	}

	template <typename FormatContext>
	auto format(const Ab::Wast::Location& location, FormatContext& ctx) {
		return fmt::format_to(
			ctx.out(), "{}:{} @{}", location.line, location.column, location.offset);
	}
};

namespace Ab::Wast {

enum class TokenKind { NAME, IDENTIFIER, NUMBER, LPAREN, RPAREN, END, ERROR };

struct Token {
	TokenKind kind;
	Range location;
	Alph* begin;
	Alph* end;  // one-past the end.
};

class Lexer {
public:
	Lexer(Location start = {0, 1, 1});

	void lex(Alph* p, Alph* pe, Alph* eof = nullptr);

	std::queue<Token>& tokens() { return output; }

	const std::queue<Token>& tokens() const { return output; }

private:
	void emit(Token tok) { output.push(tok); }

	void emit(TokenKind k, Alph* ts, Alph* te) { emit({k, {start, location}, ts, te}); }

	// Alph* p;   // current parse character
	// Alph* pe;  // end of current parse block
	// Alph* eof; // end of file pointer, or null.

	int cs;   // current state
	int act;  // internal parse helper
	// int top;   // top of state stack
	Alph* ts;  // token start
	Alph* te;  // token end

	Location start;  // token begin location
	Location location;

	std::queue<Token> output;

	// int stack[32];
};

// struct InputChunk {
// 	Alph* p;
// 	Alph* pe;
// 	Alph* eof = nullptr;
// };

// class InputChunkStream {
// };

// class TokenStream {
// public:

// 	TokenStream(Alph* p, Alph* pe, Alph* eof = nullptr) {
// 	}

// 	Token operator*() {
// 		return {TokenKind::END, {0, 0}, nullptr, nullptr};
// 	}

// 	TokenStream& operator++(int) {}

// 	void push(Token tok);

// private:
// 	Lexer lexer;
// };

}  // namespace Ab::Wast

#endif  // AB_WAST_LEXER_HPP_
