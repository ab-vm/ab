#ifndef AB_FUNC_HPP_
#define AB_FUNC_HPP_

#include <Ab/Config.hpp>
#include <Ab/Address.hpp>
#include <Ab/Types.hpp>
#include <absl/types/span.h>
#include <cstddef>
#include <utility>
#include <vector>

namespace Ab {

class Func;
class FuncInst;

struct FuncRef {};

struct InterModuleFuncRef {};

struct IntraModuleFuncRef {};

struct RefTable {};

/// Instantiated and fully resolved set of constants.
///
struct ConstPool {
	std::vector<FuncInst*> func_table;
	std::vector<float> f32_table;
	std::vector<double> f64_table;
};

/// VM Function. Shared data across multiple instantiations.
/// A module must be instantiated before it can be called. See FuncInst.
///
class Func {
public:
	Func(const FuncType* type, Byte* body, std::size_t var_nregs) noexcept
		: type_(type)
		, var_nregs_(var_nregs)
		, arg_nregs_(type->arg_nregs())
		, ret_nregs_(type->ret_nregs())
		, nregs_(var_nregs_ + arg_nregs_)
		, body_(body, std::size_t(0)) {}

	/// Pointer to the underlying type of the function.
	///
	const FuncType* type() const noexcept { return type_; }

	/// Number of x32 registers required by arguments.
	///
	std::uint32_t arg_nregs() const noexcept { return arg_nregs_; }

	/// Number of x32 registers required by return results.
	///
	std::uint32_t ret_nregs() const noexcept { return ret_nregs_; }

	/// Number of local x32 registers in function, for use as temporaries.
	///
	std::uint32_t var_nregs() const noexcept { return var_nregs_; }

	/// Total number of registers in function frame, arg + var register count.
	///
	std::uint32_t nregs() const noexcept { return nregs_; }

	/// Total number of bytes consumed by registers in this function frame.
	///
	std::uint32_t nreg_bytes() const noexcept { return nregs_ * 4; }

	/// The function body, a sequence of bytecode instructions.
	///
	Byte* body() const noexcept { return body_.data(); }

private:
	const FuncType* type_;
	std::uint32_t var_nregs_;
	std::uint32_t arg_nregs_;
	std::uint32_t ret_nregs_;
	std::uint32_t nregs_;
	absl::Span<Byte> body_;
};

/// An instantiated function.
/// The runtime data associated with a function.
///
class FuncInst {
public:
	FuncInst(Func* base) noexcept
		: base_(base)
		, arg_nregs_(base->arg_nregs())
		, ret_nregs_(base->ret_nregs())
		, var_nregs_(base->var_nregs())
		, nregs_(base->nregs())
		, body_(base->body()) {}

	FuncInst(Func* base, const ConstPool& const_pool) noexcept
		: base_(base)
		, arg_nregs_(base->arg_nregs())
		, ret_nregs_(base->ret_nregs())
		, var_nregs_(base->var_nregs())
		, nregs_(base->nregs())
		, body_(base->body())
		, const_pool_(const_pool) {}

	FuncInst(Func* base, ConstPool&& const_pool) noexcept
		: base_(base)
		, arg_nregs_(base->arg_nregs())
		, ret_nregs_(base->ret_nregs())
		, var_nregs_(base->var_nregs())
		, nregs_(base->nregs())
		, body_(base->body())
		, const_pool_(std::move(const_pool)) {}

	/// Pointer to the underlying function data, which is shared across instances.
	///
	const Func* base() const noexcept { return base_; }

	/// Pointer to the underlying type of the function.
	///
	const FuncType* type() const noexcept { return base_->type(); }

	/// Number of x32 registers required by arguments.
	///
	std::uint32_t arg_nregs() const noexcept { return arg_nregs_; }

	/// Number of x32 registers required by return results.
	///
	std::uint32_t ret_nregs() const noexcept { return ret_nregs_; }

	/// Number of local x32 registers in function, for use as temporaries.
	///
	std::uint32_t var_nregs() const noexcept { return var_nregs_; }

	/// Total number of registers in function frame, arg + var register count.
	///
	std::uint32_t nregs() const noexcept { return nregs_; }

	/// Total number of bytes consumed by registers in this function frame.
	///
	std::uint32_t nreg_bytes() const noexcept { return nregs_ * 4; }

	/// Pointer to the beginning of the function bytecodes.
	///
	Byte* body() const noexcept { return body_; }

	ConstPool& const_pool() noexcept { return const_pool_; }

	const ConstPool& const_pool() const noexcept { return const_pool_; }

	FuncInst* func_const(std::size_t index) const noexcept {
		return const_pool_.func_table[index];
	}

private:
	/// Pointer into the shared func object
	///
	const Func* base_;

	/// Number of registers initialized with arguments. Cached for locality.
	///
	std::uint32_t arg_nregs_;

	/// Number of registers taken by return result. Cached for locality.
	///
	std::uint32_t ret_nregs_;

	/// Number of registers dedicated to local variables. Cached for locality.
	///
	std::uint32_t var_nregs_;

	/// Total number of local registers, nargs + nvars; Cached for locality.
	///
	std::uint32_t nregs_;

	/// Pointer into the code section, after the declaration of the registers.
	///
	Byte* body_;

	ConstPool const_pool_;
};

}  // namespace Ab

#endif  // AB_FUNC_HPP_
