#ifndef AB_EDN_TOKEN_HPP_
#define AB_EDN_TOKEN_HPP_

#include <Ab/Assert.hpp>
#include <Ab/Parse/LineInfo.hpp>
#include <Ab/Parse/Location.hpp>

#include <cstdint>
#include <cassert>
#include <cstddef>
#include <stack>
#include <string_view>
#include <type_traits>
#include <vector>

namespace Ab::Edn {

using namespace Ab::Parse;

enum class TokenKind : std::uint8_t {
	UNINITIALIZED,
	ERROR,
	END,
	NIL,
	BOOL,
	STRING,
	KEYWORD,
	DELIMITER,
	SYMBOL,
	INTEGER,
	REAL,
	CARAT,
	HASH,
	// ()
	LPAREN,
	RPAREN,
	// []
	LBRACKET,
	RBRACKET,
	// {}
	LBRACE,
	RBRACE,
};

union TokenData {
public:
	constexpr TokenData() : dummy(0) {}

	constexpr TokenData(double real) : as_real(real) {}

	constexpr TokenData(std::intptr_t integer) : as_integer(integer) {}

	constexpr TokenData(bool b) : as_bool(b) {}

	char dummy;
	std::intptr_t as_integer;
	double as_real;
	bool as_bool;
};

class Token {
public:
	Token() = default;

	constexpr Token(TokenKind k, SrcRange range)
		: range_(range), kind_(k) {}

	constexpr Token(TokenKind k, SrcRange range, bool b)
		: range_(range), kind_(k), data_(b) {}

	constexpr Token(TokenKind k, SrcRange range, std::intptr_t integer)
		: range_(range), kind_(k), data_(integer) {}

	constexpr Token(TokenKind k, SrcRange range, double real) 
		: range_(range), kind_(k), data_(real) {}

	TokenKind kind() const noexcept { return kind_; }

	SrcRange src_range() const { return range_; }

	SrcPos src_begin() const { return range_.begin(); }

	SrcPos src_end() const { return range_.end(); }

	std::size_t length() const { return range_.length(); }

	TokenData& data() { return data_; }

	const TokenData& data() const { return data_; }

	bool bool_data() const { return data_.as_bool; }

	std::intptr_t integer_data() const { return data_.as_integer; }

	double real_data() const { return data_.as_real; }

	std::string_view strv(std::string_view buffer) const { return range_.strv(buffer); }

	/// The line/column location of the start of the token.
	///
	Location location(LineInfo& line_info) const { return line_info.location(range_.begin()); }

private:
	SrcRange range_;
	TokenData data_;
	TokenKind kind_;
};

static_assert(std::is_trivially_destructible_v<Token>, "Tokens must be trivially constructible.");

/// Deep comparsion of two tokens.
/// For many token types, there is no underlying value to compare, only the
/// location and type. If two tokens share the same location, then they must
/// share the same underlying string representation.
///
inline bool operator==(const Token& lhs, const Token& rhs) noexcept {
	if (lhs.kind() != rhs.kind()) {
	return false;
	}

	if (lhs.src_range() != rhs.src_range()) {
		return false;
	}

	switch (lhs.kind()) {
	case TokenKind::INTEGER:
		return lhs.integer_data() == rhs.integer_data();
	case TokenKind::REAL:
		return lhs.real_data() == rhs.real_data();
	case TokenKind::BOOL:
		return lhs.bool_data() == rhs.bool_data();
	case TokenKind::END:
	case TokenKind::NIL:
	case TokenKind::KEYWORD:
	case TokenKind::SYMBOL:
	case TokenKind::CARAT:
	case TokenKind::STRING:
	case TokenKind::ERROR:
	case TokenKind::LPAREN:
	case TokenKind::RPAREN:
	case TokenKind::LBRACE:
	case TokenKind::RBRACE:
	case TokenKind::LBRACKET:
	case TokenKind::RBRACKET:
		return true;
	default:
		AB_ASSERT_UNREACHABLE();
	}
	AB_ASSERT_UNREACHABLE();
}

inline bool operator!=(const Token& lhs, const Token& rhs) noexcept { return !(lhs == rhs); }

/// @group Token Construction Helpers
/// @{
///

inline Token tok_nil(SrcRange range) { return Token(TokenKind::NIL, range); }

inline Token tok_bool(SrcRange range, bool value) { return Token(TokenKind::BOOL, range, value); }

inline Token tok_end(SrcRange range) { return Token(TokenKind::END, range); }

inline Token tok_keyword(SrcRange range) { return Token(TokenKind::KEYWORD, range); }

inline Token tok_symbol(SrcRange range) { return Token(TokenKind::SYMBOL, range); }

inline Token tok_string(SrcRange range) { return Token(TokenKind::STRING, range); }

inline Token tok_integer(SrcRange range, std::intptr_t value) { return Token(TokenKind::INTEGER, range, value);  }

inline Token tok_real(SrcRange range, double value) { return Token(TokenKind::REAL, range, value); }

inline Token tok_carat(SrcRange range) { return Token(TokenKind::CARAT, range); }

/// @}
///

}  // namespace Ab::Abt

#endif  // AB_EDN_TOKEN_HPP_

