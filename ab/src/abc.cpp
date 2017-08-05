#include <ab.hpp>

#include <iostream>

using namespace ab;

auto repl(std::istream& in) -> Error {
	while (true) {
		return Error::success;
	}
};

auto printStartupBanner(std::ostream& out) -> std::ostream& {
}

extern "C" auto main() -> int {
	printStartupBanner(std::cout);
	Process::init();
	auto e = repl(std::cin);
	e = Process::kill();
	return static_cast<int>(e);
}
