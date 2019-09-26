#include <Ab/Commands.hpp>

#include <fmt/format.h>

namespace Ab {

int cmd_help([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
	// fmt::print("{}\n{}\n\n{}", SHORT_HELP_STRING, USAGE_STRING, HELP_STRING);
	return 0;
}

}  // namespace Ab
