#include <Ab/Config.hpp>
#include <Ab/Wasm/Binary/Reader.hpp>
#include <Ab/Wasm/Binary/Visitor.hpp>
#include <iostream>

extern "C" int main(int argc, char** argv) {
	Ab::Wasm::Binary::NoOpVisitor visitor{};
	Ab::Wasm::Binary::Reader reader{visitor, std::cin};
	reader();
	return EXIT_SUCCESS;
}
