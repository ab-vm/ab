#ifndef AB_WASM_BINARY_VISITOR_HPP_
#define AB_WASM_BINARY_VISITOR_HPP_

#include <Ab/Wasm/Expression.hpp>
#include <Ab/Wasm/Ir.hpp>
#include <Ab/Wasm/Section.hpp>
#include <Ab/leb128.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>

namespace Ab {
namespace Wasm {

struct FunctionType;

///  The Pure-virtual base class. See also: the No-Op base visitor.
class Visitor {
public:
	virtual ~Visitor() noexcept {}

	// Initialization

	virtual void module_start() = 0;

	virtual void module_end() = 0;

	virtual void header(::std::uint32_t magic, ::std::uint32_t version) = 0;

	/// The beginning of any section
	virtual void section_start(const Section& section) = 0;

	virtual void section_end(const Section& section) = 0;

	/// Type Section

	virtual void type_section(std::size_t count) = 0;

	virtual void function_type(const FunctionType& type) = 0;

	/// Import Section

	virtual void import_section(std::size_t count) = 0;

	virtual void import_entry(const ImportEntry& entry) = 0;

	/// Function Section

	virtual void function_section(std::size_t count) = 0;

	virtual void function_entry(std::size_t i, std::uint32_t type) = 0;

	/// Global Sectopm

	virtual void global_section(std::size_t count) = 0;

	virtual void global_entry(const GlobalType& type, const InitExpr& expr) = 0;

	/// Export Section

	virtual void export_section(std::size_t count) = 0;

	virtual void export_entry(const ExportEntry& entry) = 0;

	/// Element Section

	virtual void element_section(std::size_t count) = 0;

	virtual void element_entry(const ElementEntry& entry) = 0;

	virtual void element(const ElementEntry& entry, std::uint32_t index) = 0;

	virtual void element_entry_end(const ElementEntry& entry) = 0;

	/// Code Section

	virtual void code_section(std::size_t count) = 0;

	/// This is a weird part of the code. The function body parsing works very differently from
	/// the main parser. For now, we hand off the input to the visitor. The visitor can bring up
	/// it's own function parser and do whatever. The visitor must read to the end of the
	/// function. This is a temporary way of life.
	virtual void function_body(std::size_t index, const FunctionBody& entry, ReaderInput& in) = 0;

	virtual void function_body_end(const FunctionBody& entry) = 0;

	/// Data Section

	virtual void data_segment(const DataSegment& segment) = 0;
};

} // namespace Wasm
} // namespace Ab

#endif  // AB_WASM_BINARY_VISITOR_HPP_
