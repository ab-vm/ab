#ifndef AB_EDN_DEBUGPRINTING_HPP_
#define AB_EDN_DEBUGPRINTING_HPP_

#include <Ab/Edn/Printing.hpp>
#include <Ab/Edn/Token.hpp>
#include <Ab/Parse/Location.hpp>

#include <fmt/core.h>
#include <fmt/ostream.h>

template <>
struct fmt::formatter<Ab::Edn::TokenKind> {
	template <typename ParseContext>
	auto parse(ParseContext& cx) const {
		return cx.begin();
	}

	template <typename FormatContext>
	auto format(const Ab::Edn::TokenKind k, FormatContext& cx) const {
		std::string_view str;
		switch (k) {
		case Ab::Edn::TokenKind::LPAREN:
			str = "lparen";
			break;
		case Ab::Edn::TokenKind::RPAREN:
			str = "rparen";
			break;
		case Ab::Edn::TokenKind::LBRACE:
			str = "lbrace";
			break;
		case Ab::Edn::TokenKind::RBRACE:
			str = "rbrace";
			break;
		case Ab::Edn::TokenKind::LBRACKET:
			str = "lbracket";
			break;
		case Ab::Edn::TokenKind::RBRACKET:
			str = "rbracket";
			break;
		case Ab::Edn::TokenKind::SYMBOL:
			str = "symbol";
			break;
		case Ab::Edn::TokenKind::INTEGER:
			str = "integer";
			break;
		case Ab::Edn::TokenKind::REAL:
			str = "real";
			break;
		case Ab::Edn::TokenKind::KEYWORD:
			str = "keyword";
			break;
		case Ab::Edn::TokenKind::STRING:
			str = "string";
			break;
		case Ab::Edn::TokenKind::CARAT:
			str = "carat";
			break;
		case Ab::Edn::TokenKind::END:
			str = "end";
			break;
		default:
			str = "???";
			break;
		}
		return fmt::format_to(cx.out(), str);
	}
};

template <>
struct fmt::formatter<Ab::Edn::Token> {
	template <typename ParseContext>
	auto parse(ParseContext& cx) const {
		return cx.begin();
	}

	template <typename FormatContext>
	auto format(const Ab::Edn::Token& token, FormatContext& cx) const {
		switch (token.kind()) {
		case Ab::Edn::TokenKind::LPAREN:
		case Ab::Edn::TokenKind::RPAREN:
		case Ab::Edn::TokenKind::LBRACE:
		case Ab::Edn::TokenKind::RBRACE:
		case Ab::Edn::TokenKind::LBRACKET:
		case Ab::Edn::TokenKind::RBRACKET:
		case Ab::Edn::TokenKind::KEYWORD:
		case Ab::Edn::TokenKind::SYMBOL:
		case Ab::Edn::TokenKind::STRING:
		case Ab::Edn::TokenKind::CARAT:
		case Ab::Edn::TokenKind::END:
			return fmt::format_to(cx.out(), "({} {})", token.kind(), token.src_range());
		case Ab::Edn::TokenKind::INTEGER:
			return fmt::format_to(
				cx.out(), "(integer {} {})", token.integer_data(),
				token.src_range());
		case Ab::Edn::TokenKind::REAL:
			return fmt::format_to(
				cx.out(), "(real {} {})", token.real_data(), token.src_range());
		default:
			return fmt::format_to(
				cx.out(), "(unknown {} {})", int(token.kind()), token.src_range());
		}
	}
};

namespace Ab::Edn {

inline std::ostream& operator<<(std::ostream& out, const TokenKind k) {
	return out << fmt::format("{}", k);
}

inline std::ostream& operator<<(std::ostream& out, const Token& token) {
	return out << fmt::format("{}", token);
}

}  // namespace Ab::Edn

#endif  // AB_EDN_DEBUGPRINTING_HPP_
