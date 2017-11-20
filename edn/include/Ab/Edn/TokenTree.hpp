#ifndef AB_ABT_TOKENTREE_HPP_
#define AB_ABT_TOKENTREE_HPP_

#include <Ab/Abt/Token.hpp>
#include <Ab/Parse/Location.hpp>
#include <Ab/Abt/Lexer.hpp>

#include <memory>
#include <stack>
#include <vector>

namespace Ab::Abt {

using namespace ::Ab::Parse;

class TokenTree;
class TTNode;

using TTNodeVec = std::vector<TTNode>;

/// Group delimiter types.
///
enum class GroupDel { NONE, PAREN, BRACKET, BRACE };

using GroupIter = std::vector<TTNode>::iterator;

/// Delimited token group.
///
/// Basically, a collection of tokens wrapped in some form of bracket,
/// An example would be ( a b c ).
///
class Group {
public:
	Group() = default;

	Group(const Group&) = default;

	/// Delimiter.
	///
	GroupDel del() const noexcept { return del_; }

	GroupIter begin() { return elements_.begin(); }

	GroupIter end() { return elements_.end(); }

	TTNode& first() { return elements_[0]; }

	const TTNode& first() const { return elements_[0]; }

	TTNode& last() { return elements_[length() - 1]; }

	const TTNode& last() const { return elements_[length() - 1]; }

	std::size_t length() const { return elements_.size(); }

	inline Token& push_token(const Token& token);

	inline Group& push_group();

	TTNode& operator[](std::size_t i) {
		return elements_[i];
	}

	const TTNode& operator[](std::size_t i) const {
		return elements_[i];
	}

private:
	std::vector<TTNode> elements_;
	GroupDel del_;
};

class TokenTag {};
class GroupTag {};

constexpr TokenTag TOKEN;
constexpr GroupTag GROUP;

union TTData {
	TTData() {}

	template <typename... Args>
	TTData(TokenTag, Args&&... args) : as_token(std::forward<Args>(args)...) {}

	template <typename... Args>
	TTData(GroupTag, Args&&... args) : as_group(std::forward<Args>(args)...) {}

	~TTData() {}

	Group as_group;
	Token as_token;
};

enum class TTKind { TOKEN, GROUP };

struct TTNode {
public:
	TTNode() = delete;

	TTNode(const TTNode& other) : kind_(other.kind()) {
		switch (other.kind()) {
		case TTKind::GROUP:
			new (&data_.as_group) Group(other.as_group());
			break;
		case TTKind::TOKEN:
			new (&data_.as_token) Token(other.as_token());
			break;
		}
	}

	TTNode(TTNode&& other) : kind_(other.kind()) {
		switch (other.kind()) {
		case TTKind::GROUP:
			new (&data_.as_group) Group(std::move(other.as_group()));
			break;
		case TTKind::TOKEN:
			new (&data_.as_token) Token(std::move(other.as_token()));
			break;
		}
	}

	template <typename... Args>
	TTNode(TokenTag, Args&&... args)
		: data_(TOKEN, std::forward<Args>(args)...), kind_(TTKind::TOKEN) {}

	template <typename... Args>
	TTNode(GroupTag, Args&&... args)
		: data_(GROUP, std::forward<Args>(args)...), kind_(TTKind::GROUP) {}

	~TTNode() noexcept {
		switch (kind_) {
		case TTKind::TOKEN:
			data_.as_token.~Token();
			break;
		case TTKind::GROUP:
			data_.as_group.~Group();
			break;
		}
	}

	TTKind kind() const noexcept { return kind_; }

	Group& as_group() noexcept {
		assert(kind_ == TTKind::GROUP);
		return data_.as_group;
	}

	const Group& as_group() const noexcept {
		assert(kind_ == TTKind::GROUP);
		return data_.as_group;
	}

	Token& as_token() noexcept {
		assert(kind_ == TTKind::TOKEN);
		return data_.as_token;
	}

	const Token& as_token() const noexcept {
		assert(kind_ == TTKind::TOKEN);
		return data_.as_token;
	}

private:
	TTData data_;
	TTKind kind_;
};

inline Token& Group::push_token(const Token& token) {
	elements_.push_back(TTNode(TOKEN, token));
	return last().as_token();
}

inline Group& Group::push_group() {
	elements_.push_back(TTNode(GROUP));
	return last().as_group();
}

class TokenTree {
public:
	Group& root() noexcept { return root_; }

	const Group& root() const noexcept { return root_; }

private:
	Group root_;
};

using GroupStack = std::stack<Group*, std::vector<Group*>>;

TokenTree lex_tree(State& state, const char* in, std::size_t length) {

	TokenTree tree;
	GroupStack stack;

	stack.push(&tree.root());

	Lexer lexer(state, in, length);

	while (lexer.more()) {
		Token token = lexer.next();
		switch (token.kind()) {
		case TokenKind::DEL: {
			auto& del = token.as_del();
			switch (del.kind()) {
			case DelimiterKind::LPAREN:
			case DelimiterKind::LBRACKET:
			case DelimiterKind::LBRACE: {
				auto& parent = *stack.top();
				auto& child  = parent.push_group();
				stack.push(&child);
				child.push_token(token);
				break;
			}
			case DelimiterKind::RPAREN:
			case DelimiterKind::RBRACKET:
			case DelimiterKind::RBRACE: {
				stack.top()->push_token(token);
				if (1 < stack.size()) {
					stack.pop();
				}
				break;
			}
			case DelimiterKind::INVALID:
				assert(0);
				break;
			}
			break;
		}
		case TokenKind::KEY:
		case TokenKind::SYM:
		case TokenKind::NUM:
		case TokenKind::END:
			stack.top()->push_token(token);
			break;
		case TokenKind::ERR:
			assert(0);
			break;
		}
	}

	Token end = lexer.next();
	assert(end.kind() == TokenKind::END);
	tree.root().push_token(end);

	return tree;
}

}  // namespace Ab::Abt

#endif  // AB_ABT_TOKENTREE_HPP_
