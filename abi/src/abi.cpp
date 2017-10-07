#include <Ab/ModuleLoader.hpp>
#include <Ab/Process.hpp>
#include <Ab/Version.hpp>
#include <iostream>

namespace Abi {
struct Config {
	const char* module;
};

}  // namespace Abi

extern "C" auto main(int argc, char** argv) -> int {
	Ab::Process::init();
	Ab::ModuleLoader loader();

	std::cout << Ab::Version::ABIGAIL << std::endl;
	Ab::Process::kill();
	return 0;
};
