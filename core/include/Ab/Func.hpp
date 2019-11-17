#ifndef AB_FUNC_HPP_
#define AB_FUNC_HPP_

#include <Ab/Config.hpp>
#include <Ab/Address.hpp>
#include <cstddef>

namespace Ab {

/// VM function metadata.
///
struct FuncHeader {
	std::size_t nparams;  ///< Number of parameter slots. A slot is 32 bits.
	std::size_t nlocals;  ///< Number of local register slots. A slot is 32 bits.
};

/// VM Function. A fixed sized header of metadata, followed by the main
/// body of the function, which is linked abx bytecode.
///
class Func {
public:
	FuncHeader header;
	Byte body[];
};

static_assert(offsetof(Func, body) == 16);
static_assert(sizeof(Func) == sizeof(FuncHeader));

/// An instantiated function.
/// The runtime data associated with a function.
///
class FuncInst {
public:
	FuncInst(Func* func) : func(func), body(func->body) {}

	Func* func;
	Byte* body;
};

/// A Reference to a VM function.
///
struct FuncRef {
	std::size_t module;
	std::size_t index;
};

}  // namespace Ab

#endif  // AB_FUNC_HPP_
