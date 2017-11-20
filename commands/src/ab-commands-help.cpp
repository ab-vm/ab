#include <Ab/Commands.hpp>

#include <fmt/core.h>

namespace Ab {

const char* const SHORT_HELP_STRING =
	"ab: web assembly interpreter";

const char* const HELP_STRING =
	"subcommands:\n"
	"  ab run <wasm>              Run a WASM module.\n"
	"  ab asm <wast> [<wasm>]     Assemble a WAST input to a WASM module.\n"
	"  ab disasm <wasm> [<wast>]  Disassemble a WASM module to WAST.\n"
	"  ab help                    Print this help message.\n"
	"  ab version                 Print the version of Ab.\n";

const char* const USAGE_STRING =
	"usage: ab <command> [--] [<args>...]";

int cmd_help(int argc, char** argv) {
	fmt::print("{}\n{}\n\n{}", SHORT_HELP_STRING, USAGE_STRING, HELP_STRING);
	return 0;
}

}  // namespace Ab
