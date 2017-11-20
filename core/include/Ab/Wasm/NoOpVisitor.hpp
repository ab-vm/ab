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

	virtual void header(std::uint32_t magic, std::uint32_t version) override {}

	/// Any section

	virtual void section_start(const Section& section) override {}

	virtual void section_end(const Section& section) override {}

	/// Type Section

	virtual void type_section(std::size_t count) override {}

	virtual void function_type(const FunctionType& type) override {}

	/// Import Section

	virtual void import_section(std::size_t count) override {}

	virtual void import_entry(const ImportEntry& entry) override {}

	/// Function Section

	virtual void function_section(std::size_t count) override {}

	virtual void function_entry(std::size_t i, std::uint32_t type) override {}

	/// Global Section

	virtual void global_section(std::size_t count) override {}

	virtual void global_entry(const GlobalType& type, const InitExpr& expr) override {}

	/// Export Section

	virtual void export_section(std::size_t count) override {}

	virtual void export_entry(const ExportEntry& entry) override {}

	/// Element Section

	virtual void element_section(std::size_t count) override {}

	virtual void element_entry(const ElementEntry& entry) override {}

	virtual void element(const ElementEntry& entry, std::uint32_t index) override {}

	virtual void element_entry_end(const ElementEntry& entry) override {}

	/// Code Section

	virtual void code_section(std::size_t count) override {}

	virtual void
	function_body(std::size_t index, const FunctionBody& entry, ReaderInput& in) override {}

	virtual void function_body_end(const FunctionBody& entry) override {}

	/// Custom Section

	/// Data Section

	virtual void data_segment(const DataSegment& segment) override {}

	// virtual void customSection(const Section& section) override {}
};

}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_NOOPVISITOR_HPP_
