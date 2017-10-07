#ifndef PITH_CONSTANT_HPP_
#define PITH_CONSTANT_HPP_

#include <Pith/Config.hpp>
#include <cstddef>

namespace Pith {

/// A constant.
/// Usage:
///   ```
///   Constant<int, 0>::VALUE;
///   ```
template <typename T, T value>
struct Constant {
	using Type = T;

	static constexpr const Type VALUE = value;

	/// Instances are functors which always return their constant.
	/// Example:
	///   ```
	///   TrueConstant f;
	///   f(); // => true
	///   ```
	constexpr inline auto operator()() const noexcept -> Type {
		return VALUE;
	}
};

template <bool value>
using BoolConstant = Constant<bool, value>;

using TrueConstant = BoolConstant<true>;

using FalseConstant = BoolConstant<false>;

template <int value>
using IntConstant = Constant<int, value>;

using ZeroConstant = IntConstant<0>;

}  // namespace Pith

#endif  // PITH_CONSTANT_HPP_
