#include <Ab/Edn/Parser.hpp>
#include <cstdio>
#include <fmt/core.h>

#include <Ab/Wast/Parser.hpp>
#include <fmt/format.h>

#include <Ab/Wast/Lexer.hpp>

// #include <Ab/Wast/Parser.hpp>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace Ab {
namespace Wast {

struct Input {
	static Input open(std::string_view in) {
		Input input;

		input.fd = ::open(in.data(), O_RDONLY);
		if (input.fd < 1) {
			input.fd = -1;
			return input;
		}

		struct stat s;
		auto rc = fstat(input.fd, &s);
		if (rc != 0) {
			::close(input.fd);
			input.fd = -1;
			return input;
		}

		input.nbytes = s.st_size;

		input.buffer = (Alph*)mmap(0, input.nbytes, PROT_READ, MAP_PRIVATE, input.fd, 0);
		if (input.buffer == (void*)-1) {
			::close(input.fd);
			input.fd     = -1;
			input.buffer = nullptr;
			input.nbytes = 0;
			return input;
		}

		return input;
	}

	void close() {
		munmap((void*)buffer, nbytes);
		::close(fd);
	}

	bool ok() {
		return buffer != nullptr;
	}

	Alph* buffer       = nullptr;
	std::size_t nbytes = 0;
	int fd             = -1;
};

}  // namespace Wast

int go(Wast::Input input) {
	auto p   = input.buffer;
	auto pe  = p + input.nbytes;
	auto eof = pe;

	Wast::Lexer lexer;
	lexer.lex(p, pe, eof);

	auto& tokens = lexer.tokens();

	while(!tokens.empty()) {
		auto token = tokens.front();
		fmt::print("!(token  :range {} - {})\n", token.location.start, token.location.end);
		tokens.pop();
	}
	return 0;
}

}  // namespace Ab

using namespace Ab;

/// The base name by which the current command was called.
/// EG `/bin/example` would be `example`
///
const char* prog_name = nullptr;

/// The name by which the current command was called.
/// EG `/bin/example` would be `example`.
///
const char* call_name = nullptr;

auto compile_document(const Edn::Document& document, FILE* out) {}

auto compile_file(FILE* in, FILE* out) {}

auto load_document(FILE* in) {}

static constexpr const char* USAGE_FMT =
	"Usage: {0} [<option>...] [--] <abt> <abx>\n"
	"   Or: {0} --help\n"
	"   Or: {0} --version\n";

void print_usage(FILE* out = stderr) { fmt::print(USAGE_FMT, prog_name); }

extern "C" int main(int argc, char* argv[]) {
	const char* in  = nullptr;
	const char* out = nullptr;

	call_name = argv[0];
	prog_name = argv[0];

	int i = 1;
	while (i < argc) {
		const char* const arg = argv[i];
		if (strcmp(arg, "--") == 0) {
			++i;
			break;
		} else if(strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
			print_usage(stdout);
			exit(0);
		} else {
			break;
		}
	}

	switch (argc - i) {
	case 0:
		fmt::print(stderr, "Error: missing <abt>\n");
		print_usage(stderr);
		exit(1);
		break;
	case 1:
		break;
	case 2:
		in = argv[1];
		break;
	case 3:
		in  = argv[1];
		out = argv[2];
	default:
		fmt::print(stderr, "Error: unhandled argument: '{}'\n", argv[argc - i - 1]);
		print_usage(stderr);
		exit(1);
		break;
	}

	auto input = Wast::Input::open(in);
	if (!input.ok()) {
		fmt::print(stderr, "error: could not open file \"{}\"", in);
		return 1;
	}

	auto rc = go(input);
	input.close();

	return 0;
}
