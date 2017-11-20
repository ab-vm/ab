#ifndef AB_EDN_LEXER_HPP_
#define AB_EDN_LEXER_HPP_

#include <Ab/Edn/Error.hpp>
#include <Ab/Edn/Token.hpp>
#include <Ab/Iterator.hpp>
#include <Ab/Parse/LineInfo.hpp>

#include <string_view>

namespace Ab::Edn {

using namespace ::Ab::Parse;

double parse_real(const char* p, const char* pe);

std::intptr_t parse_integer(const char* p, const char* pe);

class Lexer {
public:
	Lexer(LineInfo& line_info, const std::string_view& str);

	Lexer(LineInfo& line_info, const char* p, const char* pe, const char* eof);

	Lexer(LineInfo& line_info, const char* p, const char* eof);

	Lexer(LineInfo& line_info);

	void reset(const char* p, const char* eof) { reset(p, eof, eof); }

	void reset(const char* p, const char* pe, const char* eof) {
		p   = p;
		pe  = eof;
		eof = eof;
	}

	Token next();

	bool more() const { return (eof != p) && (p != pe); }

	std::string_view input_buffer() const { return {block_start, size_t(pe - block_start)}; }

	std::string_view strv(const Token& token) const { return token.strv(input_buffer()); }

private:
	SrcPos ts_pos() const { return SrcPos(ts - block_start); }

	SrcPos te_pos() const { return SrcPos(te - block_start); }

	/// get the range of the current token.
	SrcRange token_range() const { return {ts_pos(), te_pos()}; }

	// line information state.
	LineInfo& line_info;
	const char* block_start;

	// parser state
	std::size_t cs;
	const char* p;
	const char* pe;
	const char* eof;

	// tokenizer state
	std::size_t act;
	const char* ts;
	const char* te;
};

class TokenEnd {};

constexpr TokenEnd TOKEN_END;

class TokenIter {
public:
	explicit TokenIter(Lexer* lexer) : lexer_(lexer), token_() { consume(); }

	TokenIter(const TokenIter&) = default;

	const Token& operator*() const { return token_; }

	const Token* operator->() const { return &token_; }

	TokenIter& operator++() {
		consume();
		return *this;
	}

	bool operator==(TokenEnd) const { return token_.kind() == TokenKind::END; }

	bool operator!=(TokenEnd) const { return token_.kind() != TokenKind::END; }

	TokenIter& operator=(const TokenIter&) = default;

	void consume() { token_ = lexer_->next(); }

	std::string_view strv(const Token& token) const { return lexer_->strv(token); }

	std::string_view strv() const { return strv(token_); }

private:
	Lexer* lexer_;
	Token token_;
};

class TokenIterAdapter {
public:
	explicit TokenIterAdapter(Lexer& lexer) : lexer_(&lexer) {}

	TokenIter begin() const { return TokenIter(lexer_); }

	TokenEnd end() const { return TOKEN_END; }

private:
	Lexer* lexer_;
};

inline TokenIterAdapter tokens(Lexer& lexer) { return TokenIterAdapter(lexer); }

}  // namespace Ab::Edn

#endif  // AB_EDN_LEXER_HPP_
