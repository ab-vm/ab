#include <Pith/Config.hpp>
#include <Pith/Version.hpp>
#include <iostream>

extern "C" int pith_main(int argc, char** argv) {
	std::cout << "Pith version " << Pith::Version::STRING << " " << Pith::Version::COMMIT
		  << std::endl;
	return 0;
}

/// When no main is defined, pith defines a main that prints version information.
// int main (int argc, char** argv) __attribute__((weak alias("pith_main")));
#pragma weak main = pith_main
