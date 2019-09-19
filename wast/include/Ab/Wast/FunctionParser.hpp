#if !defined(AB_WAST_FUNCTIONPARSER_HPP_)
#define AB_WAST_FUNCTIONPARSER_HPP_

// #include <Ab/Wast/SymbolTable.hpp>
// #include <Ab/Wast/ParseError.hpp>
#include <Ab/Wast/Location.hpp>
#include <Ab/Wast/Lexer.hpp>

namespace Ab::Wast {

struct ParseError {
	Range error;
};

class FunctionParser {
public:
	FunctionParser();

	//  FunctionParser(Location start = {0, 0, 0});

	ParseError parse(Alph* p, Alph* pe, Alph* eof = nullptr);

private:
	Lexer lexer;
};

}  // namespace Ab::Wast

#endif  // AB_WASTFUNCTION_PARSER_HPP_
