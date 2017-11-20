#ifndef AB_EDN_PARSER_HPP_
#define AB_EDN_PARSER_HPP_

#include <Ab/Edn/Ir.hpp>
#include <Ab/Edn/Lexer.hpp>
#include <Ab/Edn/Token.hpp>
#include <Ab/Edn/Error.hpp>
#include <Ab/Parse/Input.hpp>

#include <utility>

namespace Ab::Edn {

using namespace Parse;

/// The result of parsing an EDN textual input.
///
struct Document {
	NodeVector nodes;
	LineInfo line_info;
	ErrorLog error_log;
};

Document parse(const char* p, const char* pe);

Document parse(Globals& globals, const char* p, const char* pe);

std::pair<bool, Ptr<Node>> parse_node(Globals& globals, ErrorLog& log, TokenIter& it);

std::pair<bool, Ptr<Symbol>> parse_symbol(Globals& globals, ErrorLog& log, TokenIter& it);

std::pair<bool, Ptr<Symbol>> parse_keyword(Globals& globals, ErrorLog& log, TokenIter& it);

std::pair<bool, Ptr<String>> parse_string(Globals& globals, ErrorLog& log, TokenIter& it);

std::pair<bool, Ptr<List>> parse_list(Globals& globals, ErrorLog& log, TokenIter& it);

std::pair<bool, Ptr<Vector>> parse_vector(Globals& globals, ErrorLog& log, TokenIter& it);

std::pair<bool, Ptr<Map>> parse_map(Globals& globals, ErrorLog& log, TokenIter& it);

}  // namespace Ab::Edn

#endif  // AB_EDN_PARSER_HPP_
