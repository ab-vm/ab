#ifndef AB_STATE_HPP_
#define AB_STATE_HPP_

#include <Ab/Config.hpp>
#include <Ab/Address.hpp>
#include <Ab/State.hpp>
#include <cstddef>
#include <type_traits>

namespace Ab {

/// Storage for saving machine registers at interpreter entry.
/// NOTE: THIS CLASS IS WIP AND NOT USED
///
struct EntrySaveArea {
	MutAddress sp;
	MutAddress bp;
};

static_assert(std::is_standard_layout<EntrySaveArea>::value);
static_assert(offsetof(EntrySaveArea, sp) == 0);
static_assert(offsetof(EntrySaveArea, bp) == 8);
static_assert(sizeof(EntrySaveArea) == 16);

/// The current state of execution in the virtual machine.
///
/// This is a global property that can be asked of the VM. Under normal circumstances, the vm will
/// be in the RUNNING state. When an uncaught exception or other error is triggered, the VM is
/// brought down and enters the ERRORED state.
///
/// If the virtual machine hasn't been fully initialized, possibly due to a startup error, the vm
/// will be in the UNDEFINED state.
///
/// After complete and successful execution of a program, the vm is placed into
enum class ExecCond : uint8_t { UNDEFINED = 0, RUNNING, HALTED, TRAPPED, ERRORED };

enum class ExecAction : uint8_t { CRASH = 0, INTERPRET, HALT, EXIT };

/// Flags are runtime conditions located in secondary state.
///
struct Flags {
	bool trap;
	bool error;
};

static_assert(std::is_standard_layout<Flags>::value);
static_assert(offsetof(Flags, trap) == 0);
static_assert(offsetof(Flags, error) == 1);
static_assert(sizeof(Flags) == 2);

/// Primary state is cached in the interpreter, and kept in registers if possible.
/// The state is only reflected back into the struct explicitly, when needed.
///
struct ExecStateA {
	const Byte* ip;
	Byte* sp;
	FuncInst* fn;
};

static_assert(std::is_standard_layout<ExecStateA>::value);
static_assert(offsetof(ExecStateA, ip) == 0);
static_assert(offsetof(ExecStateA, sp) == 8);
static_assert(offsetof(ExecStateA, fn) == 16);
static_assert(sizeof(ExecStateA) == 24);

/// Secondary state is kept in memory and up-to-date. Frequently accessed data
/// should be placed in the primary state and cached in registers during execution.
/// Infrequently accessed state can be stored here.
///
struct ExecStateB {
	FuncInst* func;
	Byte* stack;
	ExecCond condition;
	Flags flags;
};

static_assert(std::is_standard_layout<ExecStateB>::value);
static_assert(offsetof(ExecStateB, func) == 0);
static_assert(offsetof(ExecStateB, stack) == 8);
static_assert(offsetof(ExecStateB, condition) == 16);
static_assert(offsetof(ExecStateB, flags) == 17);
static_assert(sizeof(ExecStateB) == 24);

/// Interpreter state is divided into primary and secondary state.
/// primary state is frequently accessed, and typically cached in local registers.
/// Secondary state is less frequently accessed, and normally kept up to date in memory.
///
struct ExecState {
	ExecStateA st_a;  ///< primary execution state.
	ExecStateB st_b;  ///< secondary execution state.
	EntrySaveArea entry_save_area;
};

static_assert(std::is_standard_layout<ExecState>::value);
static_assert(offsetof(ExecState, st_a) == 0);
static_assert(offsetof(ExecState, st_b) == 24);
static_assert(offsetof(ExecState, entry_save_area) == 48);
static_assert(sizeof(ExecState) == 64);

}  // namespace Ab

#endif  // AB_STATE_HPP_