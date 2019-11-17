#include <Ab/Loading.hpp>
#include <Ab/Runtime.hpp>
#include <Ab/VirtualMachine.hpp>

#include <cstdio>
#include <cstring>
#include <fmt/format.h>

const char* prog_name = nullptr;

/// Run an exported function from an on-disk abx module.
///
int run_file(const char* module_filename, std::string_view fn_name) {
	auto runtime = new Ab::Runtime();
	auto vm      = new Ab::VirtualMachine(runtime);
	Ab::Context cx(vm);

	Ab::ModuleInst* module = nullptr;

	try {
		module = Ab::instantiate_file(cx, module_filename);
	} catch (const std::runtime_error& e) {
		fmt::print(stderr, "Error loading module: '{}'\n", fn_name);
		fmt::print(stderr, "Native Exception: {}\n", e.what());
		exit(1);
	}

	AB_ASSERT(module != nullptr);

#if 0

	auto function = module->find_function(fn_name);

	if (function == nullptr) {
		fmt::print(
			stderr, "Failed to find function '{}' in '{}'\n", fn_name, module_filename);
	}

	Ab::run_function(cx, function);

#endif // 0

	return 0;
}

int run_file(const char* file) { return run_file(file, "main"); }

void print_usage(FILE* out = stderr) {
	fmt::print(out, "Usage: {} [<option>...] [--] <abx>\n", prog_name);
}

void print_help(FILE* out = stderr) { print_usage(out); }

extern "C" int main(int argc, char* argv[]) {
	prog_name = argv[0];
	int i     = 1;

	while (i < argc) {
		const char* arg = argv[i];

		if (strcmp(arg, "--") == 0) {
			++i;
			break;
		} else if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
			print_help();
			exit(0);
		} else if (arg[0] == '-') {
			fmt::print("Error: unrecognized option: '{}'\n", arg);
			print_usage();
			exit(1);
		} else {
			break;
		}

		++i;
	}

	switch (argc - i) {
	case 0:
		fmt::print(stderr, "Error: missing <abx>\n");
		exit(1);
		break;
	case 1:
		fmt::print(stderr, "executing: {}\n", argv[i]);
		run_file(argv[i]);
		break;
	default:
		fmt::print(stderr, "Error: unexpected argument '{}'\n", argv[i + 1]);
		print_usage();
		exit(1);
		break;
	}

	return 0;
}
