#ifndef AB_ModuleBUILDER_HPP_
#define AB_ModuleBUILDER_HPP_

#include <Ab/ByteBuffer.hpp>
#include <Ab/Bytes.hpp>
#include <Ab/FuncBuilder.hpp>
#include <Ab/Module.hpp>
#include <Ab/ModuleVisitation.hpp>
#include <Ab/Opcode.hpp>
#include <Ab/VectorUtilities.hpp>
#include <limits>
#include <utility>
#include <vector>

namespace Ab {

#if 0

struct ModuleNode;
struct FuncNode;

class Label {};

class LabelTable {};

class CodeNode : public CodeModel {
public:
	CodeNode() {}

	virtual ~CodeNode() noexcept override = default;

	void nregs(std::uint32_t n) noexcept { nregs_ = n; }

	std::uint32_t nregs() const noexcept { return nregs_; }

	void emit_unreachable() { buffer_.append(Opcode::UNREACHABLE); }

	void emit_halt() { buffer_.append(Opcode::HALT); }

	void emit_nop() { buffer_.append(Opcode::NOP); }

	virtual void accept(CodeVisitor& visitor) {}

	virtual void accept(InstructionListVisitor& visitor) override {
		std::size_t offset = 0;
		while (offset < buffer_.size()) {
			switch (buffer_.read<Opcode>(offset)) {
			case Opcode::HALT:
				visitor.on_halt();
				break;
			default:
				AB_ASSERT_UNREACHABLE();
				break;
			}
		}
	}

private:
	ByteBuffer buffer_;
	FuncBuilder* func_;
	std::uint32_t nregs_;
};

struct FuncNode final {
public:
	bool FuncType* type;
};

struct ModuleNode final : public ModuleModel {
	ModuleMode() = default;

	virtual ~ModuleNode() = default;

	/// @group ModuleModel
	/// @{
	///

	virtual void accept(ModuleVisitor& visitor) override {
		visitor.enter_module();
		export_section(visitor);
		func_section(visitor);
		visitor.leave_module();
	}

	/// @}
	///

	std::vector<FuncNode> funcs;
	std::vector<std::unique_ptr<Type>> types;

private:
	void accept_export_section(ModuleVisitor& visitor) {
		visitor.enter_export_section();
		visitor.leave_export_section();
	}

	void accept_func_section(ModuleVisitor& visitor) {
		visitor.enter_func_section();
		for (auto& func : funcs_) {
			return visitor.on_func(*func);
		}
		visitor.leave_func_section();
	}
};

class ModuleBuilder final {};

class Register {};

class FuncBuilder final {
public:
	FuncBuilder(std::size_t nregisters) = default;

	virtual ~FuncBuilder() = default;

	InstructionListBuilder& instruction_list() noexcept { return instruction_list_; }

	const InstructionListBuilder& instruction_list() const noexcept {
		return instruction_list_;
	}

	// Register add_register() {}

private:
	friend class ModuleBuilder;

	FuncBuilder(ModuleBuilder* module) : module_(module) {}

	ModuleBuilder* module_;
	InstructionListBuilder instruction_list;
	std::size_t nregisters_;
};

inline FuncBuilder ModuleBuilder::func() { return funcs_.emplace_back(this); }

template <typename F>
Module* build_module(F&& f) {
	ModuleNode mod;
	return write(mod);
}

template <typename F>
Module* build_fn(F&& f) {
	ModuleNode mod;
	f(push(mod.funcs()));
	return write(mod);
}

#endif

}  // namespace Ab

#endif  // AB_ModuleBUILDER_HPP_
