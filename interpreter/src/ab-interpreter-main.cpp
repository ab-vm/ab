#include <Ab/ModuleLoader.hpp>
#include <Ab/Runtime.hpp>
#include <Ab/Version.hpp>
#include <iostream>

namespace Abi {

struct Config {
	const char* module;
};

}  // namespace Abi

extern "C" int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	Ab::AutoRuntime runtime;
	Ab::ModuleLoader loader;
	std::cout << Ab::Version::ABIGAIL << std::endl;
	return 0;
};
