#ifndef AB_VIRTUALMACHINE_HPP_
#define AB_VIRTUALMACHINE_HPP_

#define AB_DEBUG

#include <Ab/Config.hpp>
#include <Ab/Assert.hpp>
#include <Ab/Debug.hpp>
#include <Ab/Interpreter.hpp>
#include <Ab/IntrusiveList.hpp>
#include <Ab/LinearMemory.hpp>
#include <Ab/Module.hpp>
#include <Ab/Runtime.hpp>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace Ab {

class Context;
class Module;
class ModuleInst;

using ContextList     = IntrusiveList<Context>;
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

template <typename T>
void set_stack_element(Byte* ptr, T x) noexcept {
	AB_DBG_MSG("setting @{}={}\n", (void*)ptr, x);
	static_assert(std::is_trivial_v<T>);
	*reinterpret_cast<T*>(ptr) = x;
}

template <typename T>
T get_stack_element(Byte* ptr) noexcept {
	AB_DBG_MSG("getting @{}\n", (void*)ptr);
	static_assert(std::is_trivial_v<T>);
	return *reinterpret_cast<const T*>(ptr);
}

template <typename... Ts, std::size_t... Is>
std::tuple<Ts...> get_stack_elements_impl(Byte* base, std::index_sequence<Is...>) {
	return std::tuple<Ts...>(get_stack_element<Ts>(base + nth_slot_offset_v<Is, Ts...>)...);
}

template <typename... Ts>
std::tuple<Ts...> get_stack_elements(Byte* base) noexcept {
	return get_stack_elements_impl<Ts...>(base, std::index_sequence_for<Ts...>());
}

template <typename... Ts, std::size_t... Is>
void set_stack_elements_impl(Byte* base, std::index_sequence<Is...>, Ts... xs) {
	(set_stack_element<Ts>(base + nth_slot_offset_v<Is, Ts...>, xs), ...);
}

template <typename... Ts>
void set_stack_elements(Byte* base, Ts... xs) noexcept {
	set_stack_elements_impl(base, std::index_sequence_for<Ts...>(), xs...);
}

/// Push a top-level-frame onto the interpreter stack.
///
/// A top-level frame is a specially tagged frame which, rather than representing
/// a call-chain, represents the entry-point into the interpreter.
///
/// The current vm-state will be saved into the frame, which means it's safe to
/// recursively enter the interpreter from native code.
///
/// @returns The frame pointer. The base of the register file for the new frame.
///
Byte* enter_native_frame(Context& cx, std::size_t nregs);

/// Remove a top-level frame from the interpreter stack.
///
/// The interpreter, when encountering a top-level frame, returns to the c-caller.
/// The c-caller is free to inspect the frame registers before clearing the frame.
/// This can be useful for obtaining return values from any nested function call.
///
/// @returns The restored frame pointer. The base of the caller's register file.
///
Byte* leave_native_frame(Context& cx, std::size_t nregs);

/// Enter a VM call from C.
///
/// The target will be interpreted. In order to call this function, you must
/// have pushed a top-level frame onto the interpreter stack.
///
/// @returns A pointer to the register vector holding the result.
///
Byte* enter_interpreter(Context& cx, FuncInst* func);

/// Call a function with arguments As returning values Rs in a tuple.
///
/// The signature will be validated at runtime, and must match the signature
/// specified through the type parameters of this function. No coercion will
/// take place.
///
/// It is safe to recursively static-call into the interpreter from a native.
///
template <typename... Rs, typename... As>
std::tuple<Rs...> static_call(Context& cx, FuncInst* func, As... as) {
	const FuncType& func_type = *func->type();
	AB_ASSERT((types_match<As...>(func_type.args)));
	AB_ASSERT((types_match<Rs...>(func_type.rets)));

	auto reg_ptr = enter_native_frame(cx, func->nregs());
	set_stack_elements<As...>(reg_ptr, as...);

	auto ret_ptr = enter_interpreter(cx, func);

	auto ret = get_stack_elements<Rs...>(ret_ptr);
	leave_native_frame(cx, func->nregs());

	return ret;
}

template <typename... Rs, typename... As>
std::tuple<Rs...> static_call(Context& cx, ModuleInst* mod_inst, std::size_t index, As... as) {
	return static_call(cx, mod_inst->func_inst(index), as...);
}

}  // namespace Ab

#endif  // AB_VIRTUALMACHINE_HPP_
