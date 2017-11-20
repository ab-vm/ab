#ifndef AB_PARSE_MATCH_HPP_
#define AB_PARSE_MATCH_HPP_

namespace Ab::Parse {

enum class ResultKind { OK, FAIL };

template <typename T>
struct Result {
	T value;
	ResultKind kind;
};

struct Succeed {};

struct Fail {};

template <typename T>
struct Value {
	template <typename I, typename... Args>
	auto operator()(I& I, Args&&... args) {
		return false;
	}

	T value;
};

template <typename T, T V>
struct Const {
	static T VALUE = V;

	template <typename Iter, typename... Args>
	auto operator()(I& in, Args&&... args) {

	}
};

template <char C>
struct Ch : public Const<char, C> {};

template <char C>
constexpr auto ch = Ch<C>();

template <typename R, typename M...> {};

template <typename R, typename M, typename... Ms>
struct Sequence {
	template <typename T>
	R operator() {
		R result = M::match(input);
		if (result) {
			result = Sequence<Ms...>::match(input);
		}
		return result;
	}
};

template <typename R, typename M>
class Sequence<R, M> {
public:
	R operator()(Input<T>& input) {
		return M::match(input);
	}
};

template <typename T>
constexpr auto sequence = Sequence<T>();

template <typename T, typename U>
inline Sequence<T, U> operator>>()

template <typename T>
class Whitespace {
public:
	operator 
};

template <typename T, typename E>
class Parser {

};

class One {};

template <typename T>
class Value {
	operator()(const char* )
};

template <typename T>
auto value(const T& value) -> Value<T> { return Value(value); }

template <typename... T>
struct Alt;

template <typename T>
struct Alt {
	
};

template <typename... Ts>
struct Seq;

template <typename T>
struct Seq<T> {
public:
	template <typename Input, typename... Args>
	operator()(Input& input, Args&&... args) const {
		match(std::forward<Input>(input), std::forward<Args>(args)...);
	}

private:
	T match;
};

template <typename T, typename... Rest>
struct Seq<T, Rest...> : public Seq<Rest..> {
	template <typename Input, typename... Args>
	auto operator()(Input&& input, Args&&... args) const {
		return match(input, args) && Seq<Rest>::operator()(input, args) {}
	}

	T match;
};

template <typename T>
struct Seq<T> {
	template <typename Input, typename... Args>
	bool operator()(Input&& input, Args&&... args) {
		return match(std::forward<Input>(input), std::forward<Args>(args)...);
	}

private:
	T match;
};

struct Action {
	template <typename Input>
	void operator(Input& in, )
};

template <typename Input, typename Parser, typename... Args>
auto parse(Input& input, Parser& parser, Args&&... args)  {
	return parser(input.begin(), input.end(), std::forward<Args>(args)...);
}

}  // namespace Ab::Parse

#endif // AB_PARSE_MATCH_HPP_

