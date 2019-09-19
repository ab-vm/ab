#include <Ab/Config.hpp>
#include <Ab/Debug.hpp>
#include <Ab/Edn/Lexer.hpp>
#include <Ab/Edn/Token.hpp>
#include <fmt/format.h>
#include <cstdint>
#include <cmath>

// The lexer generator creates some unused locals, so we disable the warning
// for this one file.
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-variable"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

/// TODO: UTF-8
/// https://git.wincent.com/wikitext.git/blob/HEAD:/ext/wikitext/wikitext_ragel.rl

namespace Ab::Edn {

// clang-format off

///
/// Integer Parser
///

%%{
	machine ab_parse_integer;

	action on_neg {
		neg = true;
	}

	action on_digit {
		val *= 10;
		val += fc - '0';
	}

	main := ( '-' @on_neg | '+' )? ( digit @on_digit )+;
}%%

%%write data;

std::intptr_t parse_integer(const char* p, const char* pe) {
	int cs;
	std::intptr_t val = 0;
	bool neg = false;

	%%write init;
	%%write exec;

	if (neg) {
		val *= -1;
	}

	AB_ASSERT_MSG(ab_parse_integer_first_final <= cs, "failed to parse integer");

	return val;
};

///
/// Real Parser
///

%%{
	machine ab_parse_real;

	action on_neg {
		neg = true;
	}

	action on_digit { 
		val *= 10;
		val += fc - '0';
	}

	action on_pt_digit {
		pt_factor /= 10;
		val += double(fc - '0') * pt_factor;
	}

	action on_ex_neg {
		ex_neg = true;
	}

	action on_ex_digit {
		ex *= 10;
		ex += fc - '0';
	}

	sign = '-' @on_neg | '+';

	digits = ( digit @on_digit )+;

	pt_digits = ( digit @on_pt_digit )+;

	pt = '.' pt_digits;

	ex_sign = '-' @on_ex_neg | '+';

	ex_digits = ( digit @on_ex_digit )+;

	ex = [eE] ex_sign? ex_digits;

	main := sign? digits ( pt ex | pt | ex );
}%%

%%write data;

double parse_real(const char* p, const char* pe) {
	int cs;

	// true if the integer has a negative sign.
	bool neg = false;
	// factor each digit of 
	double pt_factor = 1;

	// integer exponent.
	std::intptr_t ex = 0;
	bool ex_neg = false;

	double val = 0;

	%%write init;
	%%write exec;

	if (neg) {
		val *= -1;
	}

	if (ex_neg) {
		ex *= -1;
	}

	val *= pow(10, ex);

	if (cs < ab_parse_real_first_final) {
		AB_ASSERT_UNREACHABLE_MSG("failed to parse real");
	}

	return val;
};

///
/// EDN Lexer
///

%%{
	machine ab_edn_lexer;

	action on_newline {
		AB_DBG_MSG("(line break)\n");
		line_info.record_br(p - block_start + 1);
	}

	# Location Tracking machines

	newline = '\n' @on_newline;

	# Basic character sets

	sym_chars = alnum | [_\-.~];

	# Non-token machines

	comment = ';' [^\n]* newline;

	ws = ' ' | '\t' | ',' | newline;

	whitespace = ws+;

	# Token machines

	# Simple Literals

	nil = 'nil';

	true = 'true';

	false = 'false';

	# String, Symbols, Keywords

	str_char = [^"'];

	str_escape = '\\' [tnr\\"'];

	string = "\"" ( str_char | str_escape )* "\"" ;

	num_char = '.' | '+' | '-';   # chars that might start a number

	inner_only_char = '#' | ':';  # chars that cannot start a symbol

	misc_char = '.' | '*' | '!' | '_' | '?' | '$' | '%' | '&' | '=' | '<';

	special_char = num_char | inner_only_char | misc_char;

	name_char = alnum | special_char;

	# Normally, a character 
	name0 = ( alpha | misc_char ) name_char*;

	# When a symbol starts with a numeric operator, the second
	# character must be non-numeric.
	name1 = num_char ( alpha | special_char ) name_char*;

	name = name0 | name1;

	prefix = name;

	symbol = ( prefix '/' )? name;

	keyword = ':' ( prefix '/' )? name;

	# Delimiters

	lparen = "(";

	rparen = ")";

	lbracket = "[";

	rbracket = "]";

	lbrace = '{';
	
	rbrace = '}';

	# Numbers

	integer = ( '+' | '-' )? ( '0' | [1-9] digit* );

	real_pt = '.' digit+;

	real_ex = [eE] ( '+' | '-' )? digit+;

	real = ( '+' | '-' )? digit+ ( real_pt | real_ex | real_pt real_ex );

	main := |*
		nil => {
			AB_DBG_MSG("(nil)\n");
			token = tok_nil(token_range());
			token_found = true;
			fbreak;
		};

		true => {
			AB_DBG_MSG("(true)\n");
			token = tok_bool(token_range(), true);
			token_found = true;
			fbreak;
		};

		false => {
			AB_DBG_MSG("(false)\n");
			token = tok_bool(token_range(), false);
			token_found = true;
			fbreak;
		};

		real => {
			AB_DBG_MSG("(real)\n");
			token = tok_real(token_range(), parse_real(ts, te));
			token_found = true;
			fbreak;
		};

		integer => {
			auto i = parse_integer(ts, te);
			AB_DBG_MSG("(int {})\n", i);
			token = tok_integer(token_range(), i);
			token_found = true;
			fbreak;
		};

		lparen => {
			AB_DBG_MSG("(lparen)\n");
			token = {TokenKind::LPAREN, token_range()};
			token_found = true;
			fbreak;
		};

		rparen => {
			AB_DBG_MSG("(rparen)\n");
			token = {TokenKind::RPAREN, token_range()};
			token_found = true;
			fbreak;
		};

		lbracket => {
			AB_DBG_MSG("(lbracket)\n");
			token = {TokenKind::LBRACKET, token_range()};
			token_found = true;
			fbreak;
		};

		rbracket => {
			AB_DBG_MSG("(rbracket)\n");
			token = {TokenKind::RBRACKET, token_range()};
			token_found = true;
			fbreak;
		};
		
		lbrace => {
			AB_DBG_MSG("(lbrace)\n");
			token = {TokenKind::LBRACE, token_range()};
			token_found = true;
			fbreak;
		};

		rbrace => {
			AB_DBG_MSG("(rbrace)\n");
			token = {TokenKind::RBRACE, token_range()};
			token_found = true;
			fbreak;
		};
	
		keyword => {
			std::string_view str(ts, te - ts);
			AB_DBG_MSG("(keyword {})\n", str);
			token = tok_keyword(token_range());
			token_found = true;
			fbreak;
		};
	
		symbol => {
			std::string_view str(ts, te - ts);
			AB_DBG_MSG("(symbol {})\n", str);
			token =  tok_symbol(token_range());
			token_found = true;
			fbreak;
		};

		string => {
			std::string_view str(ts, te - ts);
			AB_DBG_MSG("(symbol {})\n", str);
			token =  tok_string(token_range());
			token_found = true;
			fbreak;
		};

		whitespace => {
			AB_DBG_MSG("(ws)\n");
			fgoto main;
		};

		comment => {
			AB_DBG_MSG("(comment)\n");
			fgoto main;
		};
	*|;
}%%

%% write data;

Lexer::Lexer(LineInfo& line_info, const std::string_view& str)
		: Lexer(line_info, str.begin(), str.end()) {}

Lexer::Lexer(LineInfo& line_info, const char* p, const char* pe, const char* eof)
	: line_info(line_info),
	  block_start(p),
	  p(p),
	  pe(pe),
	  eof(eof) {
	%% write init;
}

Lexer::Lexer(LineInfo& line_info, const char* p, const char* eof)
	: Lexer(line_info, p, eof, eof) {}

Lexer::Lexer(LineInfo& line_info)
	: Lexer(line_info, nullptr, nullptr) {}

Token Lexer::next() {

	if (p == pe && p == eof) {
		AB_DBG_MSG("(at-end)\n");
		auto eof_offset = p - block_start;
		SrcPos end_pos(p - block_start);
		return tok_end(SrcRange(end_pos, end_pos));
	}

	Token token;
	bool token_found = false;

	%% write exec;

	if (token_found == false) {
		AB_DBG_MSG("(no-token)\n");
		SrcPos end_pos(p - block_start);
		return tok_end(SrcRange(end_pos, end_pos));
	}

	if (cs == ab_edn_lexer_error) {
		AB_DBG_MSG("Generic lexer error\n");
		SrcPos end_pos(p - block_start);
		return tok_end(SrcRange(end_pos, end_pos));
	}

	return token;
}

// clang-format on

}  // namespace Ab::Edn
