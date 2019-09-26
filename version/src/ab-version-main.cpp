#include <Ab/Config.hpp>
#include <Ab/Version.hpp>

#include <fmt/format.h>

using namespace Ab;

extern "C" int main([[maybe_unused]] int argc, [[maybe_unused]] char*[]) {
	fmt::print("ab: version {}\n", Version::STRING);
	fmt::print("{}\n", Version::ABIGAIL);
}
