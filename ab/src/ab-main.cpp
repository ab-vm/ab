
#include <Ab/Commands.hpp>
#include <fmt/core.h>
#include <string_view>
#include <utility>
#include <vector>
#include <optional>

// https://github.com/git/git/blob/master/git.c

using BuiltinFn = int (*)(int argc, char** argv);

struct Builtin {
	const char* name;
	BuiltinFn fn;
	int options;
};

static const Builtin BUILTINS[] = {  //
	{"asm", &Ab::cmd_asm, 0},
	{"disasm", &Ab::cmd_disasm, 0},
	{"run", &Ab::cmd_run, 0},
	{"help", &Ab::cmd_help, 0},
	{"version", &Ab::cmd_version, 0}};

static const Builtin* findBuiltin(std::string_view target) {
	for (const auto& builtin : BUILTINS) {
		if (target == builtin.name) {
			return &builtin;
		}
	}
	return nullptr;
}

/// Fixup and clean the arguments.
static void processArgs(int& argc, char**& argv) {
	// place the subcommand name in argv[0]
	if (argc == 1) {
		argv[0] = strdup("help");
	} else {
		argc -= 1;
		argv += 1;
	}
}

extern "C" int main(int argc, char** argv) {
	processArgs(argc, argv);
	auto builtin = findBuiltin(std::string_view(argv[0]));
	if (!builtin) {
		fmt::print("error: unrecognized subcommand \"{}\"", argv[0]);
		return 1;
	}
	return builtin->fn(argc, argv);
}
