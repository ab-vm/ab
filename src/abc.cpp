#include "ab.hpp"
#include <iostream>

using namespace ab;

Condition repl(std::istream& in) {
	while (true) {
		return Condition::success;
	}
};

std::ostream& printStartupBanner(std::ostream& out) {
	return out << ab::programName << " " << ab::programVersion << std::endl
	           << std::endl
	           << ab::abigail << std::endl;
}

extern "C" int main() {
	printStartupBanner(std::cout);
	ab::Process::init();
	auto condition = repl(std::cin);
	condition = ab::Process::kill();
	return ab::toExitCode(condition);
}
