#ifndef AB_MODULEVISITATION_HPP_
#define AB_MODULEVISITATION_HPP_

#include <Ab/ModuleConstants.hpp>
#include <Ab/Types.hpp>
#include <vector>

namespace Ab {

class InstructionListVisitor {
public:
	virtual ~InstructionListVisitor() noexcept = default;

	virtual void on_unreachable() = 0;

	virtual void on_nop() = 0;

	virtual void on_halt() = 0;
};

class BasicInstructionListVisitor : public InstructionListVisitor {
public:
	virtual ~BasicInstructionListVisitor() override = default;

	virtual void on_unreachable() override {}

	virtual void on_nop() override {}

	virtual void on_halt() override {}
};

class InstructionListModel {
public:
	virtual ~InstructionListModel() = default;

	virtual void accept(InstructionListVisitor&) = 0;
};

/// Function visitation API.
///
class CodeVisitor {
public:
	virtual ~CodeVisitor() = default;

	// General

	virtual void enter_code(std::uint32_t nregs) = 0;

	virtual void leave_code() = 0;

	// Instructions

	virtual void on_unreachable() = 0;

	virtual void on_nop() = 0;

	virtual void on_halt() = 0;
};

/// A function visitor that does nothing--useful as a base class.
///
class BasicCodeVisitor : public CodeVisitor {
public:
	virtual ~BasicCodeVisitor() override = default;

	// General

	virtual void enter_code(std::uint32_t) override {}

	virtual void leave_code() override {}

	// Instructions

	virtual void on_unreachable() override {}

	virtual void on_nop() override {}

	virtual void on_halt() override {}
};

class CodeModel {
public:
	virtual ~CodeModel() = default;

	virtual void accept(CodeVisitor& visitor) = 0;
};

/// Module visitation API.
///
class ModuleVisitor {
public:
	virtual ~ModuleVisitor() = default;

	// General

	virtual void enter_module() = 0;

	virtual void leave_module() = 0;

	// Type Section

	virtual void enter_type_section() = 0;

	virtual void leave_type_section() = 0;

	virtual void on_type(const FuncType& type) = 0;

	// Func Section

	virtual void enter_func_section() = 0;

	virtual void leave_func_section() = 0;

	virtual void on_func(std::uint32_t type_idx) = 0;

	// Export Section

	virtual void enter_export_section() = 0;

	virtual void leave_export_section() = 0;

	// Code Section

	virtual void enter_code_section() = 0;

	virtual void leave_code_section() = 0;

	virtual void on_code(CodeModel& model) = 0;
};

/// Basic visitor that does nothing by default.
///
class BasicModuleVisitor : public ModuleVisitor {
public:
	virtual ~BasicModuleVisitor() = default;

	// General

	virtual void enter_module() override {}

	virtual void leave_module() override {}

	// Type Section

	virtual void enter_type_section() override {}

	virtual void leave_type_section() override {}

	virtual void on_type(const FuncType&) override {}

	// Func Section

	virtual void enter_func_section() override {}

	virtual void leave_func_section() override {}

	virtual void on_func(std::uint32_t) override {}

	// Export Section

	virtual void enter_export_section() override {}

	virtual void leave_export_section() override {}

	// Code Section

	virtual void enter_code_section() override {}

	virtual void leave_code_section() override {}

	virtual void on_code(CodeModel&) override {}
};

class ModuleModel {
public:
	virtual ~ModuleModel() = default;

	virtual void accept(ModuleVisitor&) = 0;
};

}  // namespace Ab

#endif  // AB_MODULEVISITATION_HPP_
