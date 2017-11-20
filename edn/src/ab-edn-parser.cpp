#define AB_DEBUG

#include <Ab/Debug.hpp>
#include <Ab/Edn/Ir.hpp>
#include <Ab/Edn/Lexer.hpp>
#include <Ab/Edn/Parser.hpp>
#include <Ab/Edn/Token.hpp>

#include <utility>

namespace Ab::Edn {

Document parse(const char* p, const char* pe) { return parse(globals, p, pe); }

Document parse(Globals& globals, const char* p, const char* pe) {
	Document document;
	Lexer lexer(document.line_info, p, pe);
	TokenIter it = tokens(lexer).begin();

	while (it != TOKEN_END) {
		auto parse = parse_node(globals, document.error_log, it);
		if (std::get<bool>(parse)) {
			document.nodes.push_back(std::get<Ptr<Node>>(parse));
		}
	}

	return document;
}

std::pair<bool, Ptr<List>> parse_list(Globals& globals, ErrorLog& log, TokenIter& it) {
	AB_ASSERT(it->kind() == TokenKind::LPAREN);
	++it;

	Ptr<List> list = node_list();

	while (true) {
		if (it == TOKEN_END) {
			log.error(it->src_range(), "Unexpected end of file while parsing list");
			break;
		}

		const auto token = *it;
		const auto kind  = token.kind();

		if (kind == TokenKind::RPAREN) {
			++it;
			break;
		}
		if (kind == TokenKind::RBRACE) {
			log.error(token.src_range(), "Unexpected '}' terminating list");
			++it;
			break;
		}
		if (kind == TokenKind::RBRACKET) {
			log.error(token.src_range(), "Unexpected ']' terminating list");
			++it;
			break;
		}

		auto parse = parse_node(globals, log, it);

		if (std::get<bool>(parse)) {
			list->push_back(std::move(std::get<Ptr<Node>>(parse)));
		}
	}

	return {true, std::move(list)};
}

std::pair<bool, Ptr<Vector>> parse_vector(Globals& globals, ErrorLog& log, TokenIter& it) {
	AB_ASSERT(it->kind() == TokenKind::LBRACKET);
	++it;

	Ptr<Vector> vector = node_vector();

	while (true) {
		if (it == TOKEN_END) {
			log.error(it->src_range(), "Unexpected end of file while parsing vector");
			break;
		}

		const auto token = *it;
		const auto kind  = token.kind();

		if (kind == TokenKind::RBRACKET) {
			++it;
			break;
		}
		if (kind == TokenKind::RBRACE) {
			log.error(token.src_range(), "Unexpected '}' terminating vector");
			++it;
			break;
		}
		if (kind == TokenKind::RPAREN) {
			log.error(token.src_range(), "Unexpected ')' terminating vector");
			++it;
			break;
		}

		auto parse = parse_node(globals, log, it);

		if (std::get<bool>(parse)) {
			vector->push_back(std::move(std::get<Ptr<Node>>(parse)));
		}
	}

	return {true, std::move(vector)};
}

static std::pair<bool, Ptr<Node>> parse_map_k(Globals& globals, ErrorLog& log, TokenIter& it) {
	if (it == TOKEN_END) {
		log.error(it->src_range(), "Unexpected end of file while parsing map key");
		return {false, nullptr};
	}

	std::pair<bool, Ptr<Node>> result(false, nullptr);
	const Token token    = *it;
	const TokenKind kind = token.kind();

	if (kind == TokenKind::RBRACE) {
		log.error(token.src_range(), "Unexpected '}' while parsing map key");
		++it;
	} else if (kind == TokenKind::RBRACE) {
		log.error(token.src_range(), "Unexpected '}' while parsing map key");
		++it;
	} else if (kind == TokenKind::RPAREN) {
		log.error(token.src_range(), "Unexpected ')' while parsing map key");
		++it;
	} else {
		result = parse_node(globals, log, it);
	}
	return result;
}

static std::pair<bool, Ptr<Node>> parse_map_v(Globals& globals, ErrorLog& log, TokenIter& it) {
	if (it == TOKEN_END) {
		log.error(it->src_range(), "Unexpected end of file while parsing map value");
		return {false, nullptr};
	}

	std::pair<bool, Ptr<Node>> result(false, nullptr);
	const Token token    = *it;
	const TokenKind kind = token.kind();

	if (kind == TokenKind::RBRACE) {
		log.error(token.src_range(), "Unexpected '}' while parsing map value");
		++it;
	} else if (kind == TokenKind::RPAREN) {
		log.error(token.src_range(), "Unexpected '}' while parsing map value");
		++it;
	} else if (kind == TokenKind::RPAREN) {
		log.error(token.src_range(), "Unexpected ')' while parsing map value");
		++it;
	} else {
		result = parse_node(globals, log, it);
	}
	return result;
}

std::pair<bool, Ptr<Map>> parse_map(Globals& globals, ErrorLog& log, TokenIter& it) {
	AB_ASSERT(it->kind() == TokenKind::LBRACE);
	++it;

	Ptr<Map> map = node_map();

	while (true) {
		if (it->kind() == TokenKind::RBRACE) {
			++it;
			break;
		}
		if (it->kind() == TokenKind::RPAREN) {
			log.error(it->src_range(), "Unexpected ')' terminating map");
			++it;
			break;
		}
		if (it->kind() == TokenKind::RBRACKET) {
			log.error(it->src_range(), "Unexpected ']' terminating vector");
			++it;
			break;
		}

		std::pair<bool, Ptr<Node>> k_parse = parse_map_k(globals, log, it);
		if (std::get<bool>(k_parse)) {
			std::pair<bool, Ptr<Node>> v_parse = parse_map_v(globals, log, it);
			if (std::get<bool>(v_parse)) {
				Ptr<Node> k = std::move(std::get<Ptr<Node>>(k_parse));
				Ptr<Node> v = std::move(std::get<Ptr<Node>>(v_parse));

				Map::Iterator search = map->find(k);

				if (search != map->end()) {
					log.error(it->src_range(), "duplicate key discovered in map");
				}
				map->insert({std::move(k), std::move(v)});
			} else {
				log.error(it->src_range(), "key not followed by value in map");
			}
		} else {
			log.error(it->src_range(), "failed to parse map key");
		}
	}

	return {true, std::move(map)};
}

std::pair<bool, Ptr<Node>> parse_node(Globals& globals, ErrorLog& log, TokenIter& it) {
	if (it == TOKEN_END) {
		return {false, nullptr};
	}

	std::pair<bool, Ptr<Node>> result(false, nullptr);
	Token token = *it;

	switch (token.kind()) {
	case TokenKind::NIL:
		result = {true, node_nil()};
		++it;
		break;
	case TokenKind::BOOL:
		++it;
		return {true, node_bool(token.bool_data())};
	case TokenKind::INTEGER:
		++it;
		return {true, node_integer(token.integer_data())};
	case TokenKind::REAL:
		++it;
		return {true, node_real(token.real_data())};
	case TokenKind::LPAREN:
		return parse_list(globals, log, it);
	case TokenKind::LBRACKET:
		return parse_vector(globals, log, it);
	case TokenKind::LBRACE:
		return parse_map(globals, log, it);
	case TokenKind::STRING:
		return parse_string(globals, log, it);
	case TokenKind::SYMBOL:
		return parse_symbol(globals, log, it);
	case TokenKind::KEYWORD:
		return parse_keyword(globals, log, it);
	case TokenKind::RPAREN:
	case TokenKind::RBRACE:
	case TokenKind::RBRACKET:
		// consume the token, report the error, but keep going.
		++it;
		log.error(token.src_range(), "unexepected closing delimiter.");
		return {false, nullptr};
	case TokenKind::END:
	default:
		AB_ASSERT_UNREACHABLE();
	}

	return result;
}

std::pair<bool, Ptr<String>> parse_string(Globals& globals, ErrorLog& log, TokenIter& it) {
	AB_ASSERT(it != TOKEN_END);
	AB_ASSERT(it->kind() == TokenKind::STRING);

	std::string str;
	for (char ch : it.strv()) {
		str.push_back(ch);
	}

	++it;

	AB_DBG_MSG("parsed string {}\n", str);
	return {true, node_string(std::move(str))};
}

std::pair<bool, Ptr<Symbol>> parse_symbol(Globals& globals, ErrorLog& log, TokenIter& it) {
	AB_ASSERT(it != TOKEN_END);
	AB_ASSERT(it->kind() == TokenKind::SYMBOL);

	std::string str;

	for (char ch : it.strv()) {
		str.push_back(ch);
	}

	++it;

	AB_DBG_MSG("parsed symbol {}\n", str);
	SymbolId id = globals.symbol_table.intern(std::move(str));
	return {true, node_symbol(id)};
}

std::pair<bool, Ptr<Symbol>> parse_keyword(Globals& globals, ErrorLog& log, TokenIter& it) {
	AB_ASSERT(it != TOKEN_END);
	AB_ASSERT(it->kind() == TokenKind::KEYWORD);

	std::string str;

	for (char ch : it.strv()) {
		str.push_back(ch);
	}

	++it;

	AB_DBG_MSG("parsed keyword {}\n", str);
	SymbolId id = globals.symbol_table.intern(std::move(str));
	return {true, node_symbol(id)};
}

}  // namespace Ab::Edn
