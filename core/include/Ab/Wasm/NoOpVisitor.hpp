#ifndef AB_WASM_NOOPVISITOR_HPP_
#define AB_WASM_NOOPVISITOR_HPP_

#include <Ab/Config.hpp>
#include <Ab/Wasm/Ir.hpp>
#include <Ab/Wasm/OpCode.hpp>
#include <Ab/Wasm/Visitor.hpp>

namespace Ab {
namespace Wasm {

///  The NoOp Visitor
class NoOpVisitor : public Visitor {
public:
	/// Initialization
	virtual ~NoOpVisitor() override {}

	virtual void module_start() override {}

	virtual void module_end() override {}

	virtual void header(std::uint32_t, std::uint32_t) override {}

	/// Any section

	virtual void section_start(const Section&) override {}

	virtual void section_end(const Section&) override {}

	/// Type Section

	virtual void type_section(std::size_t) override {}

	virtual void function_type(const FunctionType&) override {}

	/// Import Section

	virtual void import_section(std::size_t) override {}

	virtual void import_entry(const ImportEntry&) override {}

	/// Function Section

	virtual void function_section(std::size_t) override {}

	virtual void function_entry(std::size_t, std::uint32_t) override {}

	/// Global Section

	virtual void global_section(std::size_t) override {}

	virtual void global_entry(const GlobalType&, const InitExpr&) override {}

	/// Export Section

	virtual void export_section(std::size_t) override {}

	virtual void export_entry(const ExportEntry&) override {}

	/// Element Section

	virtual void element_section(std::size_t) override {}

	virtual void element_entry(const ElementEntry&) override {}

	virtual void element(const ElementEntry&, std::uint32_t) override {}

	virtual void element_entry_end(const ElementEntry&) override {}

	/// Code Section

	virtual void code_section(std::size_t) override {}

	virtual void function_body(std::size_t, const FunctionBody&, ReaderInput&) override {}

	virtual void function_body_end(const FunctionBody&) override {}

	/// Custom Section

	/// Data Section

	virtual void data_segment(const DataSegment&) override {}

	// virtual void customSection(const Section& section) override {}
};

}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_NOOPVISITOR_HPP_
