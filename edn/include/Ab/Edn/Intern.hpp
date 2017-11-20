#ifndef AB_INTERNTABLE_HPP_
#define AB_INTERNTABLE_HPP_

#include <cassert>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace Ab {

using InternId = std::size_t;

template <typename T>
struct ConstructFromId {
	T operator()(InternId id) { return T(id); }
};

template <typename T, typename I>
class InternMapIterator;

template <typename T, typename I>
class ConstInternMapIterator;

template <typename T, typename I = InternId, typename C = ConstructFromId<I>>
class InternMap {
public:
	using Value = T;
	using Identifier = I;
	using IdentifierConstructor = C;

	using Iterator = InternMapIterator<K, I>;
	using ConstIterator = ConstInternMapIterator<K, I>;

	​ bool contains(const V& value) { return _symbols.count(key) != 0; }

	​ bool contains(Symbol s) { return _strings.count(s) != 0; }

	​ Symbol intern(const std::string& s) {
		auto insertion = _symbols.insert({s, _next});
		​ if (std::get<bool>(insertion)) {
			auto id = construct_id(_next);
			assert(std::get<bool>(_strings.insert({_next, s})));
			++_next;
		}
		​ return insertion.first->second;
	}

	​ std::string to_string(const Symbol& s) {
		assert(contains(s));
		return _strings[s];
	}

	​ Symbol operator[](const std::string& s) { return intern(s); }

	​ Iterator begin() { return Iterator(_symbols.begin()); }

	​ Iterator end() { return Iterator(_symbols.end()); }

	I construct_id() { return _constructor(id); }

private:
	InternId _next = 0;
	std::unordered_map<InternId, I> _identifiers;
	std::unordered_map<InternId, V> _values;
};

template <typename T, typename I>
class InternMapIterator {
public:
	Iterator& operator++() {
		++_iter;
		return *this;
	}

	Iterator operator++(int) {
		auto copy = *this;
		_iter++;
		return copy;
	}

	​ std::pair<K, V>& operator*() const { return _iter; }

	bool operator==(const ConstIterator& rhs) const { return _iter == rhs._iter; }

	bool operator!=(const ConstIterator& rhs) const { return _iter != rhs._iter; }

private:
	friend class InternMap<K, V>;

	​ explicit Iterator(const std::unordered_map<std::string, Symbol>::iterator& iter)
		: _iter(iter) {}

	​ std::unordered_map<std::string, Symbol>::iterator _iter;
};

template <typename K, typename V>
class InternMap<K, V>::ConstIterator {
public:
	ConstIterator& operator++() {
		++_iter;
		return *this;
	}

	Iterator operator++(int) {
		auto copy = *this;
		_iter++;
		return copy;
	}

	bool operator==(const ConstIterator& rhs) const { return _iter == rhs._iter; }

	bool operator!=(const ConstIterator& rhs) const { return _iter != rhs._iter; }

private:
	friend class InternMap<K, V>;

	​ explicit ConstIterator(const std::unordered_map<std::string, Symbol>::const_iterator& iter)
		: _iter(iter) {}

	​ std::unordered_map<std::string, Symbol>::const__iterator _iter;
};

template <typename T>
class InternSet {
public:
	class Iterator;

	class ConstIterator;

	const char* operator[](const std::string& s) {
		auto it = _strings.insert(s).first;
		return (*it).c_str();
	}

	​ const char* operator[](std::string&& s) {
		auto it = _strings.insert(std::move(s)).first;
		return (*it).c_str();
	}
	​ const char* operator[](const char* s) {
		auto it = _strings.emplace(s).first;
		return (*it).c_str();
	}

private:
	std::unordered_set<T> _strings;
};

template <typename K, typename V>
class InternTable {
public:
	using Map   = std::unordered_map<K, V>;
	using Value = Map::value_type;

private:
	Map data_;
};

#endif  // AB_INTERNTABLE_HPP_
#ifndef AB_INTERNTABLE_HPP_
#define AB_INTERNTABLE_HPP_

#include <cassert>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace Ab {

template <typename K, typename V>

class InternTable {
public:
	class Iterator {
	public:
		Iterator& operator++() {
			++_iter;
			return *this;
		}
		​ Symbol& operator*() const { return _iter->second; }
		​ private : friend class SymbolTable;
		​ explicit Iterator(const std::unordered_map<std::string, Symbol>::iterator& iter)
			: _iter(iter) {}
		​ std::unordered_map<std::string, Symbol>::iterator _iter;
	};

	​ bool contains(const std::string& s) { return _symbols.count(s) != 0; }

	​ bool contains(Symbol s) { return _strings.count(s) != 0; }

	​ Symbol intern(const std::string& s) {
		auto insertion = _symbols.insert({s, _next});
		​ if (std::get<bool>(insertion)) {
			assert(std::get<bool>(_strings.insert({_next, s})));
			++_next;
		}
		​ return insertion.first->second;
	}

	​ std::string to_string(const Symbol& s) {
		assert(contains(s));
		return _strings[s];
	}

	​ Symbol operator[](const std::string& s) { return intern(s); }

	​ Iterator begin() { return Iterator(_symbols.begin()); }

	​ Iterator end() { return Iterator(_symbols.end()); }

private:
	Symbol _next = 1;
	std::unordered_map<Symbol, std::string> _strings;
	std::unordered_map<std::string, Symbol> _symbols;
};

class InternSet {
public:
	const char* operator[](const std::string& s) {
		auto it = _strings.insert(s).first;
		return (*it).c_str();
	}
	​ const char* operator[](std::string&& s) {
		auto it = _strings.insert(std::move(s)).first;
		return (*it).c_str();
	}
	​ const char* operator[](const char* s) {
		auto it = _strings.emplace(s).first;
		return (*it).c_str();
	}
	​ private : std::unordered_set<std::string> _strings;
};

template <typename K, typename V>
class InternTable {
public:
	using Map   = std::unordered_map<K, V>;
	using Value = Map::value_type;

private:
	Map data_;
};

#endif  // AB_INTERNTABLE_HPP_
