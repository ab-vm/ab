#include <Ab/Wast/Lexer.hpp>

// clang-format off

%%{

	machine ab_wast_lexer;

	# Note the begining of a token.
	action MarkStart {
		start = location;
	}

	# Location Tracking machines

	track_offset = any @{ location.offset += 1; };

	track_column  = any @{ location.column += 1; };

	track_line = "\n" @{ location.line += 1; location.column = 1; };

	track_any = track_offset | track_column | track_line;

	track = track_any*;

	any_count = any | track;

	# Basic character set tracking

	track_space = space & track;

	allowed_chars = alnum | [_\-.~];

	# Non-token machines

	long_comment = "(;" any* :>> ";)" & track;
	
	short_comment = ( ";" . ( any* ) . "\n" ) & track;

	comment = long_comment | short_comment;

	# Token machines

	# integer = ???

	# floating_point = ( ) &;

	string = ( "\"" allowed_chars+ "\"" ) & track >MarkStart;

	identifier = ( "$" allowed_chars+ ) & track >MarkStart;

	name = allowed_chars+ & track >MarkStart;

	lparen = "(" & track >MarkStart;

	rparen = ")" & track >MarkStart;

	main := |*

		lparen => {
			fmt::print("(lparen :start {})\n", location);
			emit(TokenKind::LPAREN, ts, te);
		};

		rparen => {
			fmt::print("(rparen :start {})\n", location);
			emit(TokenKind::RPAREN, ts, te);
		};
	
		"\n" => {
			fmt::print("(new-line :start)\n", location);
		};

		name => {
			std::string_view str(ts, te - ts);
			fmt::print("(name {} :start {} :end {})\n", str, start, location);
			emit(TokenKind::NAME, ts, te);
		};

		identifier => {
			std::string_view str(ts, te - ts);
			fmt::print("(indentifier {} :start {} :end {})\n", str, start, location);
			emit(TokenKind::IDENTIFIER, ts, te);
		};

		track_space+ => {
			fmt::print("(whitespace)\n");
		};
	*|;

}%%

namespace Ab::Wast {

%% write data;

Lexer::Lexer(Location start) : location(start) {
	%% write init;
}

void Lexer::lex(Alph* p, Alph* pe, Alph* eof) {
	// p = start;
	// pe = xpe;
	// eof = isEof ? pe ;
	%% write exec;
};

}  // namespace Ab::Wast
