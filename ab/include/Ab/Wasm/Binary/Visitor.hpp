#ifndef AB_WASM_BINARY_VISITOR_HPP_
#define AB_WASM_BINARY_VISITOR_HPP_

#include <Ab/Wasm/Binary/Ir.hpp>
#include <Ab/Wasm/Binary/Section.hpp>
#include <Pith/SexprPrinter.hpp>
#include <cstddef>
#include <cstdint>

namespace Ab {
namespace Wasm {
namespace Binary {

struct FunctionType;

///  The Pure-virtual base class. See also: the No-Op base visitor.
class Visitor {
public:
	virtual ~Visitor() noexcept {};

	// Initialization

	virtual auto moduleStart() -> void = 0;

	virtual auto moduleEnd() -> void = 0;

	virtual auto header(::std::uint32_t magic, ::std::uint32_t version) -> void = 0;

	/// The beginning of any section
	virtual auto sectionStart(const Section& section) -> void = 0;

	virtual auto sectionEnd(const Section& section) -> void = 0;

	/// Type Section

	virtual auto typeSection(std::size_t count) -> void = 0;

	virtual auto functionType(const FunctionType& type) -> void = 0;

	/// Import Section

	virtual auto importSection(std::size_t count) -> void = 0;

	virtual auto importEntry(const ImportEntry& entry) -> void = 0;

	/// Function Section

	virtual auto functionSection(std::size_t count) -> void = 0;

	virtual auto functionEntry(std::size_t i, std::uint32_t type) -> void = 0;

	/// Global Sectopm

	virtual auto globalSection(std::size_t count) -> void = 0;

	virtual auto globalEntry(const GlobalType& type, const Expression& expr) -> void = 0;

	/// Export Section

	virtual auto exportSection(std::size_t count) -> void = 0;

	virtual auto exportEntry(const ExportEntry& entry) -> void = 0;

	/// Element Section

	virtual auto elementSection(std::size_t count) -> void = 0;

	virtual auto elementEntry(const ElementEntry& entry) -> void = 0;

	virtual auto element(const ElementEntry& entry, std::uint32_t index) -> void = 0;

	virtual auto elementEntryEnd(const ElementEntry& entry) -> void = 0;

	/// Code Section
	virtual auto codeSection(std::size_t count) -> void = 0;

	/// This is a weird part of the code. The function body parsing works very differently from
	/// the main parser. For now, we hand off the input to the visitor. The visitor can bring up
	/// it's own function parser and do whatever. The visitor must read to the end of the
	/// function. This is a temporary way of life.
	virtual auto functionBody(std::size_t index, const FunctionBody& entry, std::istream& in)
		-> void = 0;

	virtual auto functionBodyExpression(const FunctionBody& entry, const Expression& expression)
		-> void = 0;

	virtual auto functionBodyEnd(const FunctionBody& entry) -> void = 0;
};

///  The Pure-virtual base class. See also: the No-Op base visitor.
class NoOpVisitor : public Visitor {
public:
	/// Initialization

	virtual auto moduleStart() -> void override;

	virtual auto moduleEnd() -> void override;

	virtual auto header(std::uint32_t magic, std::uint32_t version) -> void override;

	/// Any section

	virtual auto sectionStart(const Section& section) -> void override;

	virtual auto sectionEnd(const Section& section) -> void override;

	/// Type Section

	virtual auto typeSection(std::size_t count) -> void override;

	virtual auto functionType(const FunctionType& type) -> void override;

	/// Import Section

	virtual auto importSection(std::size_t count) -> void override;

	virtual auto importEntry(const ImportEntry& entry) -> void override;

	/// Function Section

	virtual auto functionSection(std::size_t count) -> void override;

	virtual auto functionEntry(std::size_t i, std::uint32_t type) -> void override;

	/// Global Section

	virtual auto globalSection(std::size_t count) -> void override {
	}

	virtual auto globalEntry(const GlobalType& type, const Expression& expr) -> void override {
	}

	/// Export Section

	virtual auto exportSection(std::size_t count) -> void override {
	}

	virtual auto exportEntry(const ExportEntry& entry) -> void override {
	}

	/// Element Section

	virtual auto elementSection(std::size_t count) -> void override {
	}

	virtual auto elementEntry(const ElementEntry& entry) -> void override {
	}

	virtual auto element(const ElementEntry& entry, std::uint32_t index) -> void override {
	}

	virtual auto elementEntryEnd(const ElementEntry& entry) -> void override {
	}

	/// Code Section

	virtual auto codeSection(std::size_t count) -> void override {
	}

	virtual auto functionBody(std::size_t index, const FunctionBody& entry, std::istream& in)
		-> void override {
	}

	virtual auto functionBodyExpression(const FunctionBody& entry, const Expression& expression)
		-> void override {
	}

	virtual auto functionBodyEnd(const FunctionBody& entry) -> void override {
	}

	/// Custom Section

	// virtual auto customSection(const Section& section) -> void override;
};

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_BINARY_VISITOR_HPP_
