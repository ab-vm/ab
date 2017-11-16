#include <Ab/Config.hpp>
#include <Ab/Wasm/Binary/Expression.hpp>
#include <Ab/Wasm/Binary/Reader.hpp>
#include <Ab/Wasm/Binary/Visitor.hpp>
#include <Pith/Maybe.hpp>
#include <Pith/SexprPrinter.hpp>
#include <Pith/StringSpan.hpp>
#include <fstream>
#include <iostream>

namespace Ab {
namespace Wasm {
namespace Binary {

const bool DEBUG = false;

/// A Binary::Visitor that dumps humand readable results to an ostream.
class WastPrinter : public NoOpVisitor {
public:
	WastPrinter(Pith::SexprPrinter& out) : out_{out} {
	}

	virtual auto moduleStart() -> void override {
		out_ << Pith::sexprStart << "module";
	}

	virtual auto moduleEnd() -> void override {
		out_ << Pith::sexprEnd;
		out_ << Pith::freshLine;
	}

	virtual auto sectionStart(const Section& section) -> void override {
		out_ << Pith::freshLine << ";;;;;;;;;;;;;;" << section;
		++(out_.indent());
	}

	virtual auto sectionEnd(const Section& section) -> void override {
		--(out_.indent());
	}

	virtual auto typeSection(std::size_t count) -> void override {
		out_ << Pith::freshLine;
		out_ << Pith::rawStart << ";; count: " << count << Pith::rawEnd;
		out_ << Pith::freshLine;
	}

	virtual auto functionType(const FunctionType& type) -> void override {
		out_ << Pith::freshLine << type;
	}

	virtual auto importSection(std::size_t count) -> void override {
		out_ << Pith::freshLine;
		out_ << Pith::rawStart << ";; count: " << count << Pith::rawEnd;
		out_ << Pith::freshLine;
	}

	virtual auto importEntry(const ImportEntry& entry) -> void override {
		out_ << Pith::freshLine << entry;
	}

	virtual auto functionSection(std::size_t count) -> void override {
		out_ << Pith::freshLine;
		out_ << Pith::rawStart << ";; count: " << count << Pith::rawEnd;
		out_ << Pith::freshLine;
		types_.reserve(count);
	}

	virtual auto functionEntry(std::size_t i, std::uint32_t type) -> void override {
		if (DEBUG) {
			out_ << Pith::freshLine;
			out_ << ";; " << Pith::sexprStart << "type" << type << Pith::sexprEnd;
		}
		types_.push_back(type);
	}

	virtual auto globalSection(std::size_t count) -> void override {
		out_ << Pith::freshLine;
		out_ << Pith::rawStart << ";; count: " << count << Pith::rawEnd;
		out_ << Pith::freshLine;
	}

	virtual auto globalEntry(const GlobalType& type, const Expression& expr) -> void override {
		out_ << Pith::freshLine;
		out_ << Pith::sexprStart << "global" << type;
		out_ << Pith::sexprStart << expr << Pith::sexprEnd;
		out_ << Pith::sexprEnd;
	}

	virtual auto exportEntry(const ExportEntry& entry) -> void override {
		out_ << Pith::freshLine << entry;
	}

	/// Element Section

	virtual auto elementSection(std::size_t count) -> void override {
	}

	virtual auto elementEntry(const ElementEntry& entry) -> void override {
		out_ << Pith::freshLine;
		out_ << Pith::sexprStart << "elem";
		out_ << Pith::sexprStart << entry.offset << Pith::sexprEnd;
	}

	virtual auto element(const ElementEntry& entry, std::uint32_t index) -> void override {
		out_ << index;
	}

	virtual auto elementEntryEnd(const ElementEntry& entry) -> void override {
		out_ << Pith::sexprEnd;
	}

	/// Code Section

	virtual auto codeSection(std::size_t count) -> void override {
		out_ << Pith::freshLine;
		out_.stream() << ";; count:" << count;
		out_.fresh(false);
	}

	virtual auto functionBody(std::size_t index, const FunctionBody& body, std::istream& in)
		-> void override {
		out_ << Pith::freshLine;
		out_ << Pith::sexprStart << "func";
		out_ << Pith::sexprStart << "type" << types_[index] << Pith::sexprEnd;
		out_ << Pith::freshLine;
		out_ << body;

		ExprReader read;
		ExprPrinter print(out_);
		read(in, print);
		// auto& ops = *body.ops;

		out_ << Pith::sexprEnd;
	}

	virtual auto functionBodyExpression(const FunctionBody& entry, const Expression& expression)
		-> void override {
		out_ << Pith::freshLine << expression;
	}

	virtual auto functionBodyEnd(const FunctionBody& entry) -> void override {
	}

private:
	std::vector<std::uint32_t> types_;
	Pith::SexprPrinter out_;
};

#if 0
inline auto disassemble(std::uint8_t* code, std::size_t n, std::ostream& out) -> void {
};

/// Disassemble WASM expressions up to (and including) the END op code.
inline auto disassemble(std::istream& in, std::ostream& out) -> void {
	// Ab::Wasm::Binary::Expr::apply
};
#endif  //

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

auto dump(std::istream& is, std::ostream& os) -> void {
	Pith::SexprPrinter out{os};
	Ab::Wasm::Binary::WastPrinter printer{out};
	Ab::Wasm::Binary::Reader reader{printer, is};

	try {
		reader();
	} catch (std::exception& e) {
		out << std::endl;
		throw;
	}
}

struct Config {
	Pith::Maybe<std::string> in;
	Pith::Maybe<std::string> out;
};

const char* name = "";

const char* const VERSION_STRING = "ab-dump version 0.0.1-danger";

const char* const USAGE =
	"Disassemble a WASM binary module.\n"
	"Usage: ab-dump [-o <output>] [--] <input>\n"
	"   Or: ab-dump --help";

auto parseArguments(Config& cfg, int argc, const char* const* argv) -> void {
	name = argv[0];

	int i = 1;
	while (i < argc) {
		Pith::StringSpan arg{argv[i]};
		if (arg == "-o" || arg == "--ouput") {
			i++;
			cfg.out = Pith::Maybe<std::string>(argv[i]);
		} else if (arg == "-h" || arg == "--help") {
			std::cout << USAGE << std::endl;
			exit(EXIT_SUCCESS);
		} else if (arg == "--version") {
			std::cout << VERSION_STRING << std::endl;
			exit(EXIT_SUCCESS);
		} else if (arg == "--") {
			i++;
			break;
		} else {
			break;
		}
		i++;
	}

	if (i < argc) {
		cfg.in = Pith::just(std::string{argv[i]});
	}

	for (int i = 1; i < argc; i++) {
		std::cout << "arg: " << argv[i] << std::endl;
	}
};

extern "C" auto main(int argc, char** argv) -> int {
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

	Ab::Wasm::Binary::UnreachableExpr e;
	Ab::Wasm::Binary::AnyExpr& any = e;
	Pith::debug_out << any;

	dump(in, out);
	return EXIT_SUCCESS;
}
