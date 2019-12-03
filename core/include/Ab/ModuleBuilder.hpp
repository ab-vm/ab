#ifndef AB_MODULEBUILDER_HPP_
#define AB_MODULEBUILDER_HPP_

#include <Ab/ByteBuffer.hpp>
#include <Ab/Bytes.hpp>
#include <Ab/FuncBuilder.hpp>
#include <Ab/Label.hpp>
#include <Ab/Module.hpp>
#include <Ab/ModuleVisitation.hpp>
#include <Ab/ModuleWriter.hpp>
#include <Ab/Opcode.hpp>
#include <Ab/VectorUtilities.hpp>
#include <limits>
#include <utility>
#include <vector>

namespace Ab {

class CodeNode;
class ModuleNode;
class FuncNode;
class FuncBuilder;
class Fixup;

enum class InstructionCategory {
	CORE,
};

enum class InsnKind {
	UNREACHABLE,
	HALT,
	NOP,
	I32_ADD,
	RETURN,
	X32_RETURN,
	X64_RETURN,
	LABEL,
};

class InsnVisitor;

class InsnNode {
public:
	virtual ~InsnNode() noexcept = 0;

	virtual InsnKind kind() const noexcept = 0;

	virtual void accept(InsnVisitor& visitor) = 0;
};

inline InsnNode::~InsnNode() noexcept = default;

class UnreachableInsnNode;
class NopInsnNode;
class HaltInsnNode;
class I32AddInsnNode;
class ReturnInsnNode;
class X32ReturnInsnNode;
class X64ReturnInsnNode;

class InsnVisitor {
public:
	virtual ~InsnVisitor() noexcept = 0;

	virtual void on_unreachable(UnreachableInsnNode& n) = 0;

	virtual void on_halt(HaltInsnNode& n) = 0;

	virtual void on_nop(NopInsnNode& n) = 0;

	virtual void on_i32_add(I32AddInsnNode& n) = 0;

	virtual void on_return(ReturnInsnNode& n) = 0;

	virtual void on_x32_return(X32ReturnInsnNode& n) = 0;

	virtual void on_x64_return(X64ReturnInsnNode& n) = 0;
};

inline InsnVisitor::~InsnVisitor() noexcept = default;

class UnreachableInsnNode final : public InsnNode {
public:
	virtual ~UnreachableInsnNode() noexcept override = default;

	virtual InsnKind kind() const noexcept override { return InsnKind::UNREACHABLE; }

	virtual void accept(InsnVisitor& v) override { v.on_unreachable(*this); }
};

class HaltInsnNode final : public InsnNode {
public:
	virtual ~HaltInsnNode() noexcept override = default;

	virtual InsnKind kind() const noexcept override { return InsnKind::HALT; }

	virtual void accept(InsnVisitor& v) override { return v.on_halt(*this); }
};

class NopInsnNode final : public InsnNode {
public:
	virtual ~NopInsnNode() noexcept override = default;

	virtual InsnKind kind() const noexcept override { return InsnKind::NOP; }

	virtual void accept(InsnVisitor& v) override { return v.on_nop(*this); }
};

class I32AddInsnNode final : public InsnNode {
public:
	I32AddInsnNode() noexcept = default;

	constexpr I32AddInsnNode(std::uint32_t dst, std::uint32_t lhs, std::uint32_t rhs) noexcept
		: dst(dst), lhs(lhs), rhs(rhs) {}

	virtual ~I32AddInsnNode() noexcept override = default;

	virtual InsnKind kind() const noexcept override { return InsnKind::I32_ADD; }

	virtual void accept(InsnVisitor& v) override { return v.on_i32_add(*this); }

	std::uint32_t dst;
	std::uint32_t lhs;
	std::uint32_t rhs;
};

class ReturnInsnNode final : public InsnNode {
public:
	constexpr ReturnInsnNode() noexcept = default;

	virtual ~ReturnInsnNode() noexcept override = default;

	virtual InsnKind kind() const noexcept override { return InsnKind::RETURN; }

	virtual void accept(InsnVisitor& v) override { return v.on_return(*this); }
};

class X32ReturnInsnNode final : public InsnNode {
public:
	constexpr X32ReturnInsnNode(std::uint32_t src) noexcept : src(src) {}

	virtual ~X32ReturnInsnNode() noexcept override = default;

	virtual InsnKind kind() const noexcept override { return InsnKind::X32_RETURN; }

	virtual void accept(InsnVisitor& v) override { return v.on_x32_return(*this); }

	std::uint32_t src;
};

class X64ReturnInsnNode final : public InsnNode {
public:
	constexpr X64ReturnInsnNode(std::uint32_t src) noexcept : src(src) {}

	virtual ~X64ReturnInsnNode() noexcept override = default;

	virtual InsnKind kind() const noexcept override { return InsnKind::X64_RETURN; }

	virtual void accept(InsnVisitor& v) override { return v.on_x64_return(*this); }

	std::uint32_t src;
};

#if 0  //////////////////////////////////////////////////////////////////////////

struct CodeMetadata {
	LabelTable label_table;
};

inline Fixup::~Fixup() noexcept {}

class LabelNode {
public:
	LabelNode() : offset_(0), pending_fixups_()
	

	virtual ~LabelNode() noexcept override = default;

	void place(std::size_t offset) {
		while (pending_fixups_.size() != 0) {
			auto fixup = pop(pending_fixups_);
			fixup.apply
		}
		for (auto fixup : pending_fixups_) {

		}
	}

	void register_pending(std::unique_ptr<Fixup>&& fixup) {

	}
private:
	std::size_t offset_;
	std::vector<std::unique_ptr<Fixup>> pending_fixups_;
	bool resolved_;
};

/// Fix up the target of a goto instruction, which has been tracked by the label table.
///
class GotoFixup final : public Fixup {
public:
	GotoFixup() noexcept = default;

	~GotoFixup() noexcept = default;

	constexpr FixupGoto(std::int32_t offset, Label label)
		: offset_(offset), label_(label) {}

	virtual void apply(CodeMetadata& data, ByteBuffer& code) const override {
		std::int32_t abs_target = data.label_table.target_of(label_);
		std::int32_t rel_target =  abs_target - offset_ - GOTO_SIZE;
		// AB_DBG_MSG("relocating goto: label-id={}, location={}, target={}\n", label_.id, offset_, rel_target);
		buffer.write<std::int32_t>(offset_ + GOTO_IMM_OFF, rel_target);
	}

private:
	std::int32_t offset_; //< the goto instruction position.
	Label label_; //< the label corresponding to the jump target.
};

#endif  /////////////////////////////////////////////////////////////////////////

class FuncNode : public CodeModel {
public:
	virtual void accept(CodeVisitor& visitor) override {
		visitor.enter_code(nregs);
		std::size_t n = insn_list.size();
		for (std::size_t i = 0; i < n; ++i) {
			InsnNode& insn = *insn_list[i];
			switch (insn.kind()) {
			case InsnKind::UNREACHABLE:
				visitor.on_unreachable();
				break;
			case InsnKind::HALT:
				visitor.on_halt();
				break;
			case InsnKind::NOP:
				visitor.on_nop();
				break;
			case InsnKind::I32_ADD: {
				auto& x = static_cast<I32AddInsnNode&>(insn);
				visitor.on_i32_add(x.dst, x.lhs, x.rhs);
				break;
			}
			case InsnKind::RETURN:
				visitor.on_return();
				break;
			case InsnKind::X32_RETURN: {
				auto& x = static_cast<X32ReturnInsnNode&>(insn);
				visitor.on_x32_return(x.src);
				break;
			}
			case InsnKind::X64_RETURN: {
				auto& x = static_cast<X64ReturnInsnNode&>(insn);
				visitor.on_x64_return(x.src);
				break;
			}
			default:
				AB_ASSERT_UNREACHABLE();
				break;
			}
		}
		visitor.leave_code();
	}

	template <typename T, typename... Args>
	void push(Args&&... args) {
		insn_list.push_back(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
	}

	std::uint32_t type_idx;
	std::uint32_t nregs;
	std::vector<std::unique_ptr<InsnNode>> insn_list;
};

class ModuleNode final : public ModuleModel {
public:
	ModuleNode() = default;

	virtual ~ModuleNode() = default;

	/// @group ModuleModel
	/// @{
	///

	virtual void accept(ModuleVisitor& visitor) override {
		visitor.enter_module();
		accept_type_section(visitor);
		accept_export_section(visitor);
		accept_func_section(visitor);
		accept_code_section(visitor);
		visitor.leave_module();
	}

	/// @}
	///

	std::span<Byte> write() {
		ModuleWriter writer;
		accept(writer);
		return writer.finalize();
	}

	std::vector<FuncNode> funcs;
	std::vector<FuncType> types;

private:
	void accept_type_section(ModuleVisitor& visitor) {
		visitor.enter_type_section();
		for (FuncType& type : types) {
			visitor.on_type(type);
		}
		visitor.leave_type_section();
	}

	void accept_export_section(ModuleVisitor& visitor) {
		visitor.enter_export_section();
		visitor.leave_export_section();
	}

	void accept_func_section(ModuleVisitor& visitor) {
		visitor.enter_func_section();
		for (const auto& func : funcs) {
			visitor.on_func(func.type_idx);
		}
		visitor.leave_func_section();
	}

	void accept_code_section(ModuleVisitor& visitor) {
		visitor.enter_code_section();
		for (auto& func : funcs) {
			visitor.on_code(func);
		}
		visitor.leave_code_section();
	}
};

#if 0   /////////////////////////////////////////////////////////////////////////

class Register {};

class FuncBuilder final {
public:
	FuncBuilder(std::size_t nregisters) = default;

	virtual ~FuncBuilder() = default;

	emit_unreachable() {}

	emit_nop() {}

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

class NopInsnNode : public Insn {}

class FuncNode {
public:
	FuncNode()
private:
	Module* module_;
	std::vector<std::unique_ptr<InsnNode>> iseq_;
};

class ModuleNode : public ModuleModel {
public:
	ModuleNode() {}

	FuncNode& emit_func() {};

	Address build() {
		ModuleWriter writer;
		accept(writer);
		return ModuleStorage(writer.finalize());
	}

	void accept()
private:
	std::vector<FuncNode> func_list_;
}
#endif  ////////////////////////////////////////////////////////////////////////

}  // namespace Ab

#endif  // AB_MODULEBUILDER_HPP_
