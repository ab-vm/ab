#ifndef AB_VIRTUALMACHINE_HPP_
#define AB_VIRTUALMACHINE_HPP_

#include <Ab/Config.hpp>
#include <Ab/Assert.hpp>
#include <Ab/Interpreter.hpp>
#include <Ab/IntrusiveList.hpp>
#include <Ab/LinearMemory.hpp>
#include <Ab/Module.hpp>
#include <Ab/Runtime.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Ab {

class Context;
class Module;
class ModuleInst;

using ContextList = IntrusiveList<Context>;
using ContextListNode = IntrusiveListNode<Context>;

/// The global state of the Abigail VM.
///
class VirtualMachine {
public:
	VirtualMachine(Runtime* runtime) : runtime_(runtime) {}

	VirtualMachine(const VirtualMachine&) = delete;

	VirtualMachine(VirtualMachine&&) noexcept = default;

	// const std::vector<Module>& modules() const { return modules_; }

	Runtime* runtime() const noexcept { return runtime_; }

	inline void enter(Context* cx);

	inline void leave(Context* cx);

	LinearMemory& linear_memory() noexcept { return linear_memory_; }

	const LinearMemory& linear_memory() const noexcept { return linear_memory_; }

	ContextList& context_list() noexcept { return context_list_; }

	const ContextList& context_list() const noexcept { return context_list_; }

private:
	Runtime* runtime_;
	std::vector<std::unique_ptr<ModuleInst>> modules_;
	LinearMemory linear_memory_;
	ContextList context_list_;
};

/// Thread-local VM context.
///
class Context {
public:
	explicit Context(VirtualMachine* vm) : vm_(vm) { enter(); }

	Context(const Context&) = delete;

	Context(Context&&) = default;

	~Context() { leave(); }

	VirtualMachine* vm() const noexcept { return vm_; }

	Runtime* runtime() const noexcept { return vm()->runtime(); }

	void enter() noexcept { vm_->enter(this); }

	void leave() noexcept { vm_->leave(this); }

	Interpreter& interpreter() noexcept { return interpreter_; }

	const Interpreter& interpreter() const noexcept { return interpreter_; }

	ExecState& exec_state() noexcept { return interpreter_.exec_state(); }

	const ExecState& exec_state() const noexcept { return interpreter_.exec_state(); }

	ContextListNode& node() noexcept { return node_; }

	const ContextListNode& node() const noexcept { return node_; }

private:
	VirtualMachine* vm_;
	Interpreter interpreter_;
	ContextListNode node_;
};

inline void VirtualMachine::enter(Context* cx) { context_list_.add(cx); }

inline void VirtualMachine::leave(Context* cx) { context_list_.remove(cx); }

}  // namespace Ab

#endif  // AB_VIRTUALMACHINE_HPP_
