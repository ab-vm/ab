#include <Ab/Config.hpp>
#include <Ab/Wasm/Binary/Reader.hpp>
#include <Ab/Wasm/Binary/Visitor.hpp>
#include <Pith/OStream.hpp>
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

	virtual auto functionBody(std::size_t index, const FunctionBody& body) -> void override {
		out_ << Pith::freshLine;
		out_ << Pith::sexprStart << "func";
		out_ << Pith::sexprStart << "type" << types_[index] << Pith::sexprEnd;
		out_ << Pith::freshLine;
		out_ << body;
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

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

auto dump(std::istream& in) -> void {
	Pith::SexprPrinter out{std::cout};
	Ab::Wasm::Binary::WastPrinter printer{out};
	Ab::Wasm::Binary::Reader reader{printer, in};

	try {
		reader();
	} catch (std::exception& e) {
		out << std::endl;
		throw;
	}
}

auto dump(const char* file) -> void {
	std::ifstream in{file, std::ios::in | std::ios::binary};
	dump(in);
}

extern "C" auto main(int argc, char** argv) -> int {
	if (argc > 1) {
		dump(argv[1]);
	} else {
		dump(std::cin);
	}

	return EXIT_SUCCESS;
}
