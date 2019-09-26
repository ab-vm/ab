#include <fmt/format.h>
#include <cstdio>
#include <cstring>

const char* prog_name = nullptr;

void print_usage(FILE* out = stderr) {
	fmt::print(out, "Usage: {} [<option>...] [--] <abx>\n", prog_name);
}

void print_help(FILE* out = stderr) {
	print_usage(out);
}

extern "C" int main(int argc, char* argv[]) {
	prog_name = argv[0];
	int i = 1;

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

	switch(argc - i) {
	case 0:
		fmt::print(stderr, "Error: missing <abx>\n");
		exit(1);
		break;
	case 1:
		fmt::print(stderr, "executing: {}\n", argv[i]);
		break;
	default:
		fmt::print(stderr, "Error: unexpected argument '{}'\n", argv[i + 1]);
		print_usage();
		exit(1);
		break;
	}

	return 0;
}
