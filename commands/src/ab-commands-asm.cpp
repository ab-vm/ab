#include <Ab/Commands.hpp>
#include <Ab/Wast/Parser.hpp>
#include <fmt/core.h>

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

int cmd_asm(int argc, char** argv) {
	const char* in  = nullptr;
	const char* out = nullptr;

	switch (argc) {
	case 1:
		break;
	case 2:
		in = argv[1];
		break;
	case 3:
		in  = argv[1];
		out = argv[2];
	default:
		fmt::print("error: too many arguments");
		return 1;
		break;
	};

	if (!in) {
		fmt::print(stderr, "error: missing input file");
		return 1;
	}

	auto input = Wast::Input::open(in);
	if (!input.ok()) {
		fmt::print(stderr, "error: could not open file \"{}\"", in);
		return 1;
	}

	auto rc = go(input);
	input.close();
	return rc;
}

}  // namespace Ab
