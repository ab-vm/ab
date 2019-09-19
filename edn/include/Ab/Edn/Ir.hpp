#ifndef AB_ABT_IR_HPP_
#define AB_ABT_IR_HPP_

#include <Ab/Assert.hpp>
#include <Ab/HashUtilities.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Ab::Edn {

class Node;
class Nil;
class Bool;
class Integer;
class Real;
class Symbol;
class Keyword;
class Character;
class String;
class List;
class Vector;
class Map;
class Set;

/// Alias to shared pointer. A wrapper type, used to obscure the
/// fact that we are using shared pointer.
///
template <typename T>
using Ptr = std::shared_ptr<T>;

/// Create a pointer to an object, who's lifetime will be managed via reference counting.
///
template <typename T, typename... Args>
Ptr<T> construct(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

/// The set of node types.
///
enum class Kind {
	NIL,
	BOOL,
	INTEGER,
	REAL,
	SYMBOL,
	KEYWORD,
	CHARACTER,
	STRING,
	LIST,
	VECTOR,
	MAP,
	SET
};

/// The root class of all IR types.
/// * Nodes are reference-counted via the Ptr type.
/// * Nodes provide a simple mechanism for type reflection via the kind() member function.
/// * Nodes can be downcast to concrete node types (eg Vec, Str) via node_cast.
/// * Every object in EDN may have associated metadata.
///
class Node {
public:
	Kind kind() const noexcept { return kind_; }

	const Ptr<Map>& metadata() const { return metadata_; }

	void set_metadata(const Ptr<Map>& metadata) { metadata_ = metadata; }

	void set_metadata(Ptr<Map>&& metadata) { metadata_ = std::move(metadata); }

protected:
	Node(Kind kind) : kind_(kind) {}

	Node(Kind kind, const Ptr<Map>& metadata) : kind_(kind), metadata_(metadata) {}

	Node(Kind kind, Ptr<Map>&& metadata) : kind_(kind), metadata_(std::move(metadata)) {}

private:
	Kind kind_;
	Ptr<Map> metadata_;
};

/// @group Node casting and conversion
/// @{
///

/// Assert that the node kind matches T::KIND.
/// Every Node type defines a KIND constant.
///
template <typename T>
void assert_kind(const Node& node) {
	AB_ASSERT(node.kind() == T::KIND);
}

/// In order to cast a Node to type T, T must define the static T::KIND.
/// If node->kind() does not match T::KIND, then the cast will abort at runtime.
///
template <typename T>
Ptr<T> node_cast(const Ptr<Node>& node) {
	assert_kind<T>(*node);
	return std::static_pointer_cast<T>(node);
}

template <typename T>
Ptr<T> node_cast(const Ptr<const Node>& node) {
	static_assert(std::is_const_v<T>, "Cannot cast a const node to a non-const value.");
	assert_kind<T>(*node);
	return std::static_pointer_cast<T>(node);
}

template <typename T>
Ptr<T> node_cast(Ptr<Node>&& node) {
	assert_kind<T>(*node);
	return std::static_pointer_cast<T>(std::move(node));
}

template <typename T>
Ptr<T> node_cast(Ptr<const Node>&& node) {
	static_assert(std::is_const_v<T>, "Cannot cast a const node to a non-const value.");
	assert_kind<T>(*node);
	return std::static_pointer_cast<T>(std::move(node));
}

/// @}
///

/// @group Node Utilities.
/// @{
///

/// Hash a node object.
///
inline std::size_t node_hash(const Node& node);

/// Compare two nodes for equality.
///
inline bool node_eq(const Node& lhs, const Node& rhs);

/// @}
///

/// @group Implementation types.
/// @{

/// Helper for computing a node's hash from a Ptr<Node>.
/// This functor is suitable for use as a Hash in standard collections.
///
struct NodeHash {
	std::size_t operator()(const Ptr<Node>& node) const { return node_hash(*node); }

	std::size_t operator()(const Ptr<const Node>& node) const { return node_hash(*node); }
};

/// Helper for determining if two nodes are equal, from Ptr<Node>.
/// This functor is suitable for use as KeyEqual in standard collections (eg std::map<Ptr<Node>>).
///
struct NodeEq {
	bool operator()(const Ptr<Node>& lhs, const Ptr<Node>& rhs) const {
		return node_eq(*lhs, *rhs);
	}

	bool operator()(const Ptr<const Node>& lhs, const Ptr<const Node>& rhs) const {
		return node_eq(*lhs, *rhs);
	}
};

/// A native hashtable of reference counted nodes.
/// Rather than hashing and comparing pointers, the node map examines the
/// underlying nodes.
///
using NodeMap = std::unordered_map<Ptr<Node>, Ptr<Node>, NodeHash, NodeEq>;

using NodePair = std::pair<Ptr<Node>, Ptr<Node>>;

using NodeVector = std::vector<Ptr<Node>>;

/// A helper subclass of Node, which defines the static KIND value.
/// The KIND is used to check node downcasts at runtime.
///
template <Kind K>
class NodeType : public Node {
public:
	static constexpr Kind KIND = K;

protected:
	NodeType() : Node(KIND) {}

	NodeType(const Ptr<Map>& metadata) : Node(KIND, metadata) {}

	NodeType(Ptr<Map>&& metadata) : Node(KIND, std::move(metadata)) {}
};

/// @}
///

/// @group Core Node Types
/// @{
///

/// nil value.
///
class Nil : public NodeType<Kind::NIL> {
public:
	Nil() {}

	Nil(const Ptr<Map>& metadata) : NodeType(metadata) {}

	Nil(Ptr<Map>&& metadata) : NodeType(std::move(metadata)) {}

	constexpr bool operator==(const Nil&) const { return true; }

	constexpr bool operator!=(const Nil&) const { return false; }

	constexpr std::size_t hash() const { return Ab::hash(Kind::NIL); }
};

/// Boolean value, true or false.
///
class Bool : public NodeType<Kind::BOOL> {
public:
	Bool(bool value) : value_(value) {}

	Bool(const Ptr<Map>& metadata, bool value) : NodeType(metadata), value_(value) {}

	Bool(Ptr<Map>&& metadata, bool value) : NodeType(std::move(metadata)), value_(value) {}

	bool value() const { return value_; }

	void set_value(bool value) { value_ = value; }

	bool operator==(const Bool& rhs) const { return value() == rhs.value(); }

	bool operator!=(const Bool& rhs) const { return value() != rhs.value(); }

	std::size_t hash() const { return Ab::hash(std::size_t(Kind::BOOL), value_); }

private:
	bool value_;
};

/// Integer literal.
///
class Character : public NodeType<Kind::CHARACTER> {
public:
	Character(char value) : value_(value) {}

	Character(const Ptr<Map>& metadata, bool value) : NodeType(metadata), value_(value) {}

	Character(Ptr<Map>&& metadata, bool value) : NodeType(std::move(metadata)), value_(value) {}

	char value() const { return value_; }

	void set_value(char value) { value_ = value; }

	bool operator==(const Character& rhs) const { return value_ == rhs.value_; }

	bool operator!=(const Character& rhs) const { return value_ != rhs.value_; }

	std::size_t hash() const { return Ab::hash(std::size_t(KIND), value_); }

private:
	std::intptr_t value_;
};

/// Integer literal.
///
class Integer : public NodeType<Kind::INTEGER> {
public:
	Integer(std::intptr_t value) : value_(value) {}

	Integer(const Ptr<Map>& metadata, std::intptr_t value)
		: NodeType(metadata), value_(value) {}

	Integer(Ptr<Map>&& metadata, std::intptr_t value)
		: NodeType(std::move(metadata)), value_(value) {}

	std::intptr_t value() const { return value_; }

	void set_value(std::intptr_t value) { value_ = value; }

	bool operator==(const Integer& rhs) const { return value_ == rhs.value_; }

	bool operator!=(const Integer& rhs) const { return value_ != rhs.value_; }

	std::size_t hash() const { return Ab::hash(std::size_t(Kind::INTEGER), value_); }

private:
	std::intptr_t value_;
};

/// Floating-point / Real number.
///
class Real : public NodeType<Kind::REAL> {
public:
	Real(double value) : value_(value) {}

	Real(const Ptr<Map>& metadata, double value) : NodeType(metadata), value_(value) {}

	Real(Ptr<Map>&& metadata, double value) : NodeType(std::move(metadata)), value_(value) {}

	double value() const { return value_; }

	void set_value(double value) { value_ = value; }

	bool operator==(const Real& rhs) const { return value_ == rhs.value_; }

	bool operator!=(const Real& rhs) const { return value_ != rhs.value_; }

	std::size_t hash() const { return Ab::hash(std::size_t(Kind::REAL), value_); }

private:
	double value_;
};

using SymbolId = std::uintptr_t;

/// Symbol object.
///
class Symbol : public NodeType<Kind::SYMBOL> {
public:
	Symbol(SymbolId id) : id_(id) {}

	Symbol(const Ptr<Map>& metadata, SymbolId id) : NodeType(metadata), id_(id) {}

	Symbol(Ptr<Map>&& metadata, SymbolId id) : NodeType(metadata), id_(id) {}

	SymbolId id() const { return id_; }

	void set_id(SymbolId id) { id_ = id; }

	bool operator==(const Symbol& rhs) const { return id_ == rhs.id_; }

	bool operator!=(const Symbol& rhs) const { return id_ != rhs.id_; }

	std::size_t hash() const { return Ab::hash(std::size_t(Kind::SYMBOL), id_); }

private:
	SymbolId id_;
};

using KeywordId = std::size_t;

/// Keyword symbol object.
///
class Keyword : public NodeType<Kind::KEYWORD> {
public:
	Keyword(KeywordId id) : id_(id) {}

	Keyword(const Ptr<Map>& metadata, KeywordId id) : NodeType(metadata), id_(id) {}

	Keyword(Ptr<Map>&& metadata, KeywordId id) : NodeType(metadata), id_(id) {}

	KeywordId id() const { return id_; }

	void set_id(KeywordId id) { id_ = id; }

	bool operator==(const Keyword& rhs) const { return id_ == rhs.id_; }

	bool operator!=(const Keyword& rhs) const { return id_ != rhs.id_; }

	std::size_t hash() const { return Ab::hash(std::size_t(Kind::KEYWORD), id_); }

private:
	KeywordId id_;
};

/// String Literal.
///
class String : public NodeType<Kind::STRING> {
public:
	String(const std::string& value) : value_(value) {}

	const std::string& value() const { return value_; }

	void set_value(const std::string& value) { value_ = value; }

	bool operator==(const String& rhs) const { return value_ == rhs.value_; }

	bool operator!=(const String& rhs) const { return value_ != rhs.value_; }

	std::size_t hash() const { return Ab::hash(std::size_t(Kind::STRING), value_); }

private:
	std::string value_;
};

/// List, a sequence of elements.
/// While lists are distinguishable from vectors via kind(), the underlying
/// implementation is the same.
///
class List : public NodeType<Kind::LIST> {
public:
	using Iterator      = NodeVector::iterator;
	using ConstIterator = NodeVector::const_iterator;

	List() {}

	List(std::initializer_list<Ptr<Node>> list) : elements_(list) {}

	std::vector<Ptr<Node>>& elements() { return elements_; }

	const std::vector<Ptr<Node>>& elements() const { return elements_; }

	Ptr<Node>& at(std::size_t i) { return elements_.at(i); }

	const Ptr<Node>& at(std::size_t i) const { return elements_.at(i); }

	std::size_t size() const { return elements_.size(); }

	/// Deep comparison of the vector's elements.
	///
	bool operator==(const List& rhs) const {
		std::size_t sz = size();
		if (sz != rhs.size()) {
			return false;
		}
		for (std::size_t i = 0; i < sz; ++i) {
			if (!node_eq(*at(i), *rhs.at(i))) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const List& rhs) const { return !(*this == rhs); }

	std::size_t hash() const {
		std::size_t h = mix(HASH_SEED, std::size_t(Kind::LIST));
		for (const auto& ptr : elements_) {
			h = mix(h, node_hash(*ptr));
		}
		return h;
	}

	Iterator begin() { return elements_.begin(); }

	ConstIterator begin() const { return elements_.begin(); }

	Iterator end() { return elements_.end(); }

	ConstIterator end() const { return elements_.end(); }

	ConstIterator cbegin() const { return elements_.begin(); }

	ConstIterator cend() const { return elements_.end(); }

	void push_back(const Ptr<Node>& value) { elements_.push_back(value); }

	void push_back(Ptr<Node>&& value) { elements_.push_back(std::move(value)); }

private:
	NodeVector elements_;
};

/// Vector, a contiguous array of node elements.
/// constant-time indexing.
///
class Vector : public NodeType<Kind::VECTOR> {
public:
	using Iterator      = NodeVector::iterator;
	using ConstIterator = NodeVector::const_iterator;

	Vector() {}

	Vector(std::initializer_list<Ptr<Node>> list) : elements_(list) {}

	std::vector<Ptr<Node>>& elements() { return elements_; }

	const std::vector<Ptr<Node>>& elements() const { return elements_; }

	Ptr<Node>& operator[](std::size_t i) { return elements_[i]; }

	const Ptr<Node>& operator[](std::size_t i) const { return elements_[i]; }

	Ptr<Node>& at(std::size_t i) { return elements_.at(i); }

	const Ptr<Node>& at(std::size_t i) const { return elements_.at(i); }

	std::size_t size() const { return elements_.size(); }

	/// Deep comparison of the vector's elements.
	///
	bool operator==(const Vector& rhs) const {
		std::size_t sz = size();
		if (sz != rhs.size()) {
			return false;
		}
		for (std::size_t i = 0; i < sz; ++i) {
			if (!node_eq(*at(i), *rhs.at(i))) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const Vector& rhs) const { return !(*this == rhs); }

	std::size_t hash() const {
		std::size_t h = mix(HASH_SEED, std::size_t(Kind::VECTOR));
		for (const auto& ptr : elements_) {
			h = mix(h, node_hash(*ptr));
		}
		return h;
	}

	Iterator begin() { return elements_.begin(); }

	ConstIterator begin() const { return elements_.begin(); }

	Iterator end() { return elements_.end(); }

	ConstIterator end() const { return elements_.end(); }

	ConstIterator cbegin() const { return elements_.cbegin(); }

	ConstIterator cend() const { return elements_.cend(); }

	void push_back(const Ptr<Node>& value) { elements_.push_back(value); }

	void push_back(Ptr<Node>&& value) { elements_.push_back(std::move(value)); }

private:
	NodeVector elements_;
};

/// Map: a dictionary of node->node.
/// Users beware: if a key is mutated, that entry is no longer retrievable.
///
class Map : public NodeType<Kind::MAP> {
public:
	using ValueType     = std::pair<const Ptr<Node>, Ptr<Node>>;
	using Iterator      = NodeMap::iterator;
	using ConstIterator = NodeMap::const_iterator;

	Map() {}

	Map(std::initializer_list<ValueType> list) : elements_(list) {}

	NodeMap& elements() { return elements_; }

	const NodeMap& elements() const { return elements_; }

	Ptr<Node>& at(const Ptr<Node>& key) { return elements_.at(key); }

	const Ptr<Node>& at(const Ptr<Node>& key) const { return elements_.at(key); }

	Iterator find(const Ptr<Node>& key) { return elements_.find(key); }

	ConstIterator find(const Ptr<Node>& key) const { return elements_.find(key); }

	void insert(const ValueType& value) { elements_.insert(value); }

	void insert(ValueType&& value) { elements_.insert(std::move(value)); }

	std::size_t size() const { return elements_.size(); }

	/// Deep comparison of the maps' elements.
	///
	bool operator==(const Map& rhs) const {
		if (size() != rhs.size()) {
			return false;
		}

		// try to find all [k,v] in the rhs map.
		for (const std::pair<const Ptr<Node>, Ptr<Node>>& element : elements_) {
			const Ptr<Node>& key = element.first;
			const Ptr<Node>& val = element.second;

			NodeMap::const_iterator search = rhs.elements_.find(key);

			if (search == rhs.elements_.end()) {
				return false;  // failed to find key
			}

			const Ptr<Node>& rhs_val = search->second;

			if (!node_eq(*val, *rhs_val)) {
				return false;  // key was found but value was not-equal.
			}
		}

		return true;
	}

	bool operator!=(const Map& rhs) const { return !(*this == rhs); }

	std::size_t hash() const {
		auto h = HASH_SEED;
		for (const auto& element : elements_) {
			h = mix(h, node_hash(*element.first));   // key
			h = mix(h, node_hash(*element.second));  // val
		}
		return h;
	}

	Iterator begin() { return elements_.begin(); }

	ConstIterator begin() const { return elements_.begin(); }

	Iterator end() { return elements_.end(); }

	ConstIterator end() const { return elements_.end(); }

	ConstIterator cbegin() const { return elements_.begin(); }

	ConstIterator cend() const { return elements_.end(); }

private:
	NodeMap elements_;
};

/// Map: a dictionary of node->node.
/// TODO: implement sets
///
class Set : public NodeType<Kind::SET> {
private:
	std::unordered_set<Ptr<Node>> elements_;
};

/// @}
///

/// @group node casting helpers
/// @{
///

/// Cast node to a specific type. If the type is mismatched, abort.
///
template <typename T>
T& as(Node& node) {
	assert_kind<T>(node);
	return static_cast<T&>(node);
}

template <typename T>
const T& as(const Node& node) {
	assert_kind<T>(node);
	return static_cast<const T&>(node);
}

template <typename T>
const T& as_const(const Node& node) {
	assert_kind<T>(node);
	return static_cast<const T&>(node);
}

inline Nil& as_nil(Node& node) { return as<Nil>(node); }

inline const Nil& as_nil(const Node& node) { return as<Nil>(node); }

inline Bool& as_bool(Node& node) { return as<Bool>(node); }

inline const Bool& as_bool(const Node& node) { return as<Bool>(node); }

inline Character& as_character(Node& node) { return as<Character>(node); }

inline const Character& as_character(const Node& node) { return as<Character>(node); }

inline Integer& as_integer(Node& node) { return as<Integer>(node); }

inline const Integer& as_integer(const Node& node) { return as<Integer>(node); }

inline Real& as_real(Node& node) { return as<Real>(node); }

inline const Real& as_real(const Node& node) { return as<Real>(node); }

inline Symbol& as_symbol(Node& node) { return as<Symbol>(node); }

inline const Symbol& as_symbol(const Node& node) { return as<Symbol>(node); }

inline Keyword& as_keyword(Node& node) { return as<Keyword>(node); }

inline const Keyword& as_keyword(const Node& node) { return as<Keyword>(node); }

inline String& as_string(Node& node) { return as<String>(node); }

inline const String& as_string(const Node& node) { return as<String>(node); }

inline List& as_list(Node& node) { return as<List>(node); }

inline const List& as_list(const Node& node) { return as<List>(node); }

inline Vector& as_vector(Node& node) { return as<Vector>(node); }

inline const Vector& as_vector(const Node& node) { return as<Vector>(node); }

inline Map& as_map(Node& node) { return as<Map>(node); }

inline const Map& as_map(const Node& node) { return as<Map>(node); }

inline Set& as_set(Node& node) { return as<Set>(node); }

inline const Set& as_set(const Node& node) { return as<Set>(node); }

/// @}
///

/// Deep node comparison, testing for equality.
/// Nodes lack a polymorphic/virtual comparison, so this is implemented via
/// switching and recursion.
///
inline bool node_eq(const Node& lhs, const Node& rhs) {
	if (lhs.kind() != rhs.kind()) {
		return false;
	}
	switch (lhs.kind()) {
	case Kind::NIL:
		return as_nil(lhs) == as_nil(rhs);
	case Kind::BOOL:
		return as_bool(lhs) == as_bool(rhs);
	case Kind::INTEGER:
		return as_integer(lhs) == as_integer(rhs);
	case Kind::REAL:
		return as_real(lhs) == as_real(rhs);
	case Kind::SYMBOL:
		return as_symbol(lhs) == as_symbol(rhs);
	case Kind::KEYWORD:
		return as_keyword(lhs) == as_keyword(rhs);
	case Kind::CHARACTER:
		return as_character(lhs) == as_character(rhs);
	case Kind::STRING:
		return as_string(lhs) == as_string(rhs);
	case Kind::LIST:
		return as_list(lhs) == as_list(rhs);
	case Kind::VECTOR:
		return as_vector(lhs) == as_vector(rhs);
	case Kind::MAP:
		return as_map(lhs) == as_map(rhs);
	// TODO implement set.
	default:
		AB_ASSERT_UNREACHABLE();
	}
}

/// Deep node comparison, testing for inequality.
///
inline bool node_ne(const Node& lhs, const Node& rhs) { return !node_eq(lhs, rhs); }

/// Computes a node's hash.
///
inline std::size_t node_hash(const Node& node) {
	switch (node.kind()) {
	case Kind::NIL:
		return as_nil(node).hash();
	case Kind::BOOL:
		return as_bool(node).hash();
	case Kind::INTEGER:
		return as_integer(node).hash();
	case Kind::REAL:
		return as_real(node).hash();
	case Kind::SYMBOL:
		return as_symbol(node).hash();
	case Kind::KEYWORD:
		return as_keyword(node).hash();
	case Kind::STRING:
		return as_string(node).hash();
	case Kind::LIST:
		return as_list(node).hash();
	case Kind::VECTOR:
		return as_vector(node).hash();
	case Kind::MAP:
		return as_map(node).hash();
	default:
		AB_ASSERT_UNREACHABLE();
	}
}

/// Dispatch to F based on node's type.
///
template <typename F, typename... Args>
inline auto node_dispatch(const Node& node, F& f, Args&&... args) {
	switch (node.kind()) {
	case Kind::NIL:
		return f.on_nil(as_nil(node), std::forward<Args>(args)...);
	case Kind::BOOL:
		return f.on_bool(as_bool(node), std::forward<Args>(args)...);
	case Kind::INTEGER:
		return f.on_integer(as_integer(node), std::forward<Args>(args)...);
	case Kind::REAL:
		return f.on_real(as_real(node), std::forward<Args>(args)...);
	case Kind::SYMBOL:
		return f.on_symbol(as_symbol(node), std::forward<Args>(args)...);
	case Kind::KEYWORD:
		return f.on_keyword(as_keyword(node), std::forward<Args>(args)...);
	case Kind::STRING:
		return f.on_string(as_string(node), std::forward<Args>(args)...);
	case Kind::LIST:
		return f.on_list(as_list(node), std::forward<Args>(args)...);
	case Kind::VECTOR:
		return f.on_vector(as_vector(node), std::forward<Args>(args)...);
	case Kind::MAP:
		return f.on_map(as_map(node), std::forward<Args>(args)...);
	default:
		AB_ASSERT_UNREACHABLE();
	}
}

/// @group Metadata manipulation
/// @{
///

/// Store metadata into a node.
/// @returns the pointer to the node.
template <typename T>
Ptr<T>& set_metadata(Ptr<T>& node, const Ptr<Map>& metadata) {
	node->set_metadata(metadata);
	return node;
}

template <typename T>
Ptr<T>& set_metadata(Ptr<T>& node, Ptr<Map>&& metadata) {
	node->set_metadata(std::move(metadata));
	return node;
}

template <typename T>
Ptr<T>&& set_metadata(Ptr<T>& node, const Ptr<Map>& metadata) {
	node->set_metadata(metadata);
	return std::move(node);
}

template <typename T>
Ptr<T>&& set_metadata(Ptr<T>&& node, Ptr<Map>&& metadata) {
	node->set_metadata(std::move(metadata));
	return std::move(node);
}

template <typename T>
const Ptr<T>& set_metadata(const Ptr<T>& node, const Ptr<Map>& metadata) {
	node->set_metadata(metadata);
	return node;
}

template <typename T>
const Ptr<T>& set_metadata(const Ptr<T>& node, Ptr<Map>&& metadata) {
	node->set_metadata(std::move(metadata));
	return node;
}

/// @}
///

/// String and Symbol interning table.
///
class SymbolTable {
public:
	SymbolTable() = default;

	SymbolTable(const SymbolTable&) = default;

	SymbolTable(SymbolTable&&) = default;

	/// Associate str with a unique id. If str already has an associated
	/// id, return it. Otherwise, allocate a new id.
	///
	SymbolId intern(const std::string& str) {
		auto search = symbols_.find(str);

		if (search != symbols_.end()) {
			// string was already interned.
			return search->second;
		}

		// String hasn't been interned before, assign a new id.
		auto id = next_;
		symbols_.insert({str, id});
		strings_.insert({id, str});
		++next_;
		return id;
	}

	/// Get the id of a string which has already been interned.
	/// Throws a runtime error if the string hasn't been interned.
	///
	SymbolId id(const std::string& str) const { return symbols_.at(str); }

	/// True if str has an associated ID.
	///
	bool interned(const std::string& str) const { return symbols_.count(str) != 0; }

	const std::string& string(SymbolId id) const { return strings_.at(id); }

private:
	SymbolId next_ = 0;

	/// str->sym map. get a symbol id via a string.
	///
	std::unordered_map<std::string, std::size_t> symbols_;

	/// sym->str map. get a string via a symbol id.
	///
	std::unordered_map<std::size_t, std::string> strings_;
};

struct Globals {
	SymbolTable symbol_table;
};

extern Globals globals;

/// @group Node Constructors
/// @{
///

inline Ptr<Nil> node_nil() { return construct<Nil>(); }

inline Ptr<Bool> node_bool(bool value) { return construct<Bool>(value); }

inline Ptr<Integer> node_integer(std::intptr_t value) { return construct<Integer>(value); }

inline Ptr<Real> node_real(double value) { return construct<Real>(value); }

inline Ptr<Symbol> node_symbol(SymbolId id) { return construct<Symbol>(id); }

inline Ptr<Symbol> node_symbol(Globals& globals, const std::string& string) {
	return construct<Symbol>(globals.symbol_table.intern(string));
}

inline Ptr<Symbol> node_symbol(const std::string& string) { return node_symbol(globals, string); }

inline Ptr<Keyword> node_keyword(SymbolId id) { return construct<Keyword>(id); }

inline Ptr<Keyword> node_keyword(Globals& globals, const std::string& string) {
	return node_keyword(globals.symbol_table.intern(string));
}

inline Ptr<Keyword> node_keyword(const std::string& string) {
	return node_keyword(globals, string);
}

inline Ptr<String> node_string(const std::string& value) { return construct<String>(value); }

inline Ptr<List> node_list() { return construct<List>(); }

inline Ptr<List> node_list(std::initializer_list<Ptr<Node>> list) { return construct<List>(list); }

inline Ptr<Vector> node_vector(std::initializer_list<Ptr<Node>> list) {
	return construct<Vector>(list);
}

inline Ptr<Vector> node_vector() { return construct<Vector>(); }

inline Ptr<Map> node_map() { return construct<Map>(); }

inline Ptr<Map> node_map(std::initializer_list<Map::ValueType> list) {
	return construct<Map>(list);
}

/// @}
///

/// @group Symbol helpers.
/// @{
///

/// Convert a symbol or keyword to it's string representation.
///
inline Ptr<String> string_of(Globals& globals, Ptr<Symbol> symbol) {
	return construct<String>(globals.symbol_table.string(symbol->id()));
}

inline Ptr<String> string_of(Ptr<Symbol> symbol) { return string_of(globals, symbol); }

/// @}
///

}  // namespace Ab::Edn

#endif  // AB_ABT_IR_HPP_
