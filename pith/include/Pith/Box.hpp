#ifndef PITH_BOX_HPP_
#define PITH_BOX_HPP_

#include <Pith/Config.hpp>
#include <utility>

namespace Pith {

/// A single-value container.
/// While Box is very simple, it lays the foundation for more complex data types.
///
/// Usage:
///   ```
///   Box<int> i = 1;
///   i = 3;
///   v(); // 3
///   ```
template <typename T>
class Box {
public:
	using Type = T;

	template <typename U>
	inline constexpr Box(U&& x) : value_(x) {
	}

	inline auto operator()() && -> Type&& {
		return value_;
	}

	inline auto operator()() & -> Type& {
		return value_;
	}

	inline constexpr auto operator()() const & -> const Type& {
		return value_;
	}

	template <typename Other>
	auto operator=(const Other& x) -> Box<Type>& {
		value_ = x;
		return *this;
	}

	template <typename Other>
	auto operator=(Other&& x) -> Box<Type>& {
		value_ = std::move(x);
		return *this;
	}

protected:
	Type value_;
};

}  // namespace Pith

#endif  // PITH_BOX_HPP_
