#include <Ab/Commands.hpp>
#include <Ab/Version.hpp>
#include <fmt/core.h>

namespace Ab {

int cmd_version(int argc, char** argv) {
	fmt::print("ab version {}", Ab::Version::STRING);
	return 0;
}

}  // namespace Ab
