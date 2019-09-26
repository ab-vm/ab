#include <unistd.h>
#include <fmt/format.h>
#include <cstdio>

const char* const SHORT_HELP_STRING =
	"ab: a language runtime toolkit";

const char* const USAGE_STRING =
	"usage: ab <command> [--] [<args>...]";

const char* const HELP_STRING =
	"subcommands:\n"
	"  ab run <abx>             Execute an abx module.\n"
	"  ab asm <abt> [<out>]     Assemble an abt module into abx.\n"
	"  ab disasm <abx> [<out>]  Disassemble an abx module into abt.\n"
	"  ab help                  Print this help message.\n"
	"  ab version               Print the version of Ab.";

extern "C" int main(int argc, char* argv[]) {
	switch (argc) {
	case 1:
		fmt::print("{}\n", SHORT_HELP_STRING);
		fmt::print("{}\n", USAGE_STRING);
		fmt::print("{}\n", HELP_STRING);
		return 0;
	case 2:
		execlp("man", "man", argv[1], NULL);
		fmt::print(stderr, "failed to exec man\n");
		return 2;
	default:
		fmt::print(stderr, "too many arguments to help command\n");
		return 1;
	}
	return 0;
}
