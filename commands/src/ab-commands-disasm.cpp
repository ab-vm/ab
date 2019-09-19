#include <Ab/Config.hpp>
#include <Ab/Maybe.hpp>
#include <Ab/Sexpr.hpp>
#include <Ab/StringSpan.hpp>
#include <Ab/Wasm/ExprPrinter.hpp>
#include <Ab/Wasm/Expression.hpp>
#include <Ab/Wasm/Ir.hpp>
#include <Ab/Wasm/NoOpVisitor.hpp>
#include <Ab/Wasm/Printing.hpp>
#include <Ab/Wasm/Reader.hpp>
#include <fstream>
#include <iostream>

const char* name = "";

const char* const VERSION_STRING = "ab-dump version 0.0.1-danger";

const char* const USAGE =
	"Disassemble a WASM binary module.\n"
	"Usage: ab-dump [-o <output>] [--] <input>\n"
	"   Or: ab-dump --help";

struct Config {
	Ab::Maybe<std::string> in  = Ab::NOTHING;
	Ab::Maybe<std::string> out = Ab::NOTHING;
	bool debug                 = false;
	bool verbose               = false;
};

namespace Ab {
namespace Wasm {

const bool DEBUG = false;

/// A Binary::Visitor that dumps human readable results to an ostream.
class WastPrinter : public NoOpVisitor {
public:
	WastPrinter(const Config& cfg, Ab::Sexpr::Formatter& out) : cfg_{cfg}, out_{out} {}

	virtual auto module_start() -> void override { out_ << Ab::Sexpr::START << "module"; }

	virtual auto module_end() -> void override {
		out_ << Ab::Sexpr::END;
		out_ << Ab::Sexpr::FRESH;
	}

	virtual auto section_start(const Section& section) -> void override {
		if (cfg_.verbose) {
			out_ << Ab::Sexpr::FRESH << ";;;;;;;;;;;;;;" << section;
			++(out_.indent());
		}
	}

	virtual auto section_end(const Section&) -> void override {
		if (cfg_.verbose) {
			--(out_.indent());
		}
	}

	virtual auto type_section(std::size_t count) -> void override {
		if (cfg_.verbose) {
			out_ << Ab::Sexpr::FRESH;
			out_ << fmt::format(";; count: {}", count);
			out_ << Ab::Sexpr::FRESH;
		}
	}

	virtual auto function_type(const FunctionType& type) -> void override {
		out_ << Ab::Sexpr::FRESH << type;
	}

	virtual auto import_section(std::size_t count) -> void override {
		if (cfg_.verbose) {
			out_ << Ab::Sexpr::FRESH;
			out_ << fmt::format(";; count: {}", count);
			out_ << Ab::Sexpr::FRESH;
		}
	}

	virtual auto import_entry(const ImportEntry& entry) -> void override {
		out_ << Ab::Sexpr::FRESH << entry;
	}

	virtual auto function_section(std::size_t count) -> void override {
		if (cfg_.verbose) {
			out_ << Ab::Sexpr::FRESH;
			out_ << fmt::format(";; count: {}", count);
			out_ << Ab::Sexpr::FRESH;
		}
		types_.reserve(count);
	}

	virtual auto function_entry(std::size_t, std::uint32_t type) -> void override {
		if (DEBUG) {
			out_ << Ab::Sexpr::FRESH;
			out_ << ";; " << Ab::Sexpr::START << "typezzz" << type << Ab::Sexpr::END;
		}
		types_.push_back(type);
	}

	virtual auto global_section(std::size_t count) -> void override {
		if (cfg_.verbose) {
			out_ << Ab::Sexpr::FRESH;
			out_ << fmt::format(";; count: {}", count);
			out_ << Ab::Sexpr::FRESH;
		}
	}

	virtual auto global_entry(const GlobalType& type, const InitExpr& expr) -> void override {
		out_ << Ab::Sexpr::FRESH;
		out_ << Ab::Sexpr::START << "global" << type;
		out_ << Ab::Sexpr::START << expr << Ab::Sexpr::END;
		out_ << Ab::Sexpr::END;
	}

	virtual auto export_entry(const ExportEntry& entry) -> void override {
		out_ << Ab::Sexpr::FRESH << entry;
	}

	/// Element Section

	virtual auto element_section(std::size_t) -> void override {}

	virtual auto element_entry(const ElementEntry& entry) -> void override {
		out_ << Ab::Sexpr::FRESH;
		out_ << Ab::Sexpr::START << "elem";
		out_ << Ab::Sexpr::START << entry.offset << Ab::Sexpr::END;
	}

	virtual auto element(const ElementEntry&, std::uint32_t index) -> void override {
		out_ << index;
	}

	virtual auto element_entry_end(const ElementEntry&) -> void override {
		out_ << Ab::Sexpr::END;
	}

	/// Code Section

	virtual auto code_section(std::size_t count) -> void override {
		if (cfg_.verbose) {
			out_ << Ab::Sexpr::FRESH;
			out_ << fmt::format(";; count: {}", count);
			out_.fresh(false);
		}
	}

	virtual auto function_body(std::size_t index, const FunctionBody& body, ReaderInput& in)
		-> void override {
		out_ << Ab::Sexpr::FRESH;
		out_ << Ab::Sexpr::START << "func";
		out_ << Ab::Sexpr::START << "typezz" << fmt::format("{}", (types_[index]))
		     << Ab::Sexpr::END;
		out_ << Ab::Sexpr::FRESH;
		out_ << body;

		ExprReader read;
		ExprPrinter print(out_);
		read(in, body.size, print);
		// auto& ops = *body.ops;

		out_ << Ab::Sexpr::END;
	}

	virtual auto function_body_end(const FunctionBody&) -> void override {}

	virtual auto data_segment(const DataSegment& segment) -> void override {
		out_ << Ab::Sexpr::FRESH << segment;
	}

	const Ab::Sexpr::Formatter& buffer() const { return out_; }

private:
	const Config& cfg_;
	std::vector<std::uint32_t> types_;
	Ab::Sexpr::Formatter out_;
};

}  // namespace Wasm

auto dump(Config& cfg, std::istream& is, std::ostream& os) -> void {
	Ab::Sexpr::Formatter out;
	Ab::Wasm::WastPrinter printer{cfg, out};
	Ab::Wasm::Reader reader{printer, is};
	reader();
	os << printer.buffer().get();
}

auto parseArguments(Config& cfg, int argc, const char* const* argv) -> void {
	name  = argv[0];
	int i = 1;

	// Flags and non-positional arguments

	while (i < argc) {
		Ab::StringSpan arg{argv[i]};
		if (arg == "-o" || arg == "--ouput") {
			i++;
			cfg.out = Ab::Maybe<std::string>(argv[i]);
		} else if (arg == "-h" || arg == "--help") {
			std::cout << USAGE << std::endl;
			exit(EXIT_SUCCESS);
		} else if (arg == "--version") {
			std::cout << VERSION_STRING << std::endl;
			exit(EXIT_SUCCESS);
		} else if (arg == "--debug" || arg == "-d") {
			cfg.debug = true;
		} else if (arg == "--verbose" || arg == "-v") {
			cfg.verbose = true;
		} else if (arg == "--") {
			i++;
			break;
		} else {
			break;
		}
		i++;
	}

#if 0
	for (option : OPTION_TABLE) {
		if (option.matches(argv[index])) {
			option.handle(argc, argv, index);
		}
	}
#endif  //

	// Positional arguments

	if (i < argc) {
		cfg.in = Ab::just(std::string{argv[i]});
		i++;
	}
};

int cmd_disasm(int argc, char** argv) {
	Config cfg;

	parseArguments(cfg, argc, argv);

	std::ifstream infile;
	std::streambuf* inbuffer = std::cin.rdbuf();
	if (cfg.in) {
		infile.open(*cfg.in, std::ios::in | std::ios::binary);
		inbuffer = infile.rdbuf();
	}
	std::istream in(inbuffer);

	std::ofstream outfile;
	std::streambuf* outbuffer = std::cout.rdbuf();
	if (cfg.out) {
		outfile.open(*cfg.out, std::ios::out | std::ios::binary);
		outbuffer = outfile.rdbuf();
	}
	std::ostream out(outbuffer);

	dump(cfg, in, out);
	return EXIT_SUCCESS;
}

}  // namespace Ab
