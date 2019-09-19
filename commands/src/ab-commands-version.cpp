#include <Ab/Commands.hpp>
#include <Ab/Version.hpp>
#include <fmt/format.h>

namespace Ab {

int cmd_version([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	fmt::print("ab version {}", Ab::Version::STRING);
	return 0;
}

}  // namespace Ab
