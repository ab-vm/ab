namespace Ab {
namespace Boot {

class JitBuilder {

};


class WasmBuilder : public Wasm::NoOpVisitor {

};

struct 
class 
class InterpreterBuilder {
public:
	InterpreterBuilder(Builder& builder) {

	}
};

struct StringArg {
	using ValueType = std::pair<std::string, bool>;
	struct Action {
		operator()(ValueType& out) {
			
		}
	}
};

template <typename Arg, template <typename Action>... Flag>
struct Option {
	Flag<Arg::Action>... flag
};

template <char C>
template <typename Action>
struct Short {
	static const char character = C;
};

using OutputFileOption = Option<StringArg, Short<"o">, Long<"option">>

ShortOptionMatcher("o") {

}

template <typename Action>
struct Parser {

};

class Result {}

class ParseState {
	int argc;
	char** argv;
	char* current;
	std::size_t offset;
};

class Parser {
	Parser(int argc, char** argv) : 
	operator() {
		for (const auto& option : options) {
			if ()
		}
	}
}
} //
} //