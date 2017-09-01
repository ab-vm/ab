#include <Pith/Config.hpp>
#include <Pith/Version.hpp>
#include <iostream>

extern "C" auto pith_main([[gnu::unused]] int argc, [[gnu::unused]] char** argv) -> int {
	std::cout << "Pith version " << Pith::Version::STRING << " " << Pith::Version::COMMIT
		  << std::endl;
	return 0;
}

/// When no main is defined, pith defines a main that prints version information.
// int main (int argc, char** argv) __attribute__((weak alias("pith_main")));
#pragma weak main = pith_main
