#ifndef PITH_BOX_HPP_
#define PITH_BOX_HPP_

#include <Pith/Config.hpp>
#include <Pith/InPlace.hpp>
#include <utility>

namespace Pith {

/// A single-value container.
/// While Box is very simple, it lays the foundation for more complex data types.
///
/// Usage:
///   ```
///   Box<int> i = 1;
///   i = 3;
///   i(); // 3
///   ```
template <typename T> class Box {
public:
	using Type = T;

	/// Forwarding constructor.
	template <typename... Args>
	explicit constexpr inline Box(const InPlace&, Args&&... args)
		: value_{std::forward<Args>(args)...} {
	}

	explicit constexpr inline Box(Type& other) : value_{other} {
	}

	explicit constexpr inline Box(const Type& other) : value_{other} {
	}

	explicit constexpr inline Box(Type&& other) : value_{std::move(other)} {
	}

	/// Copy constructor.
	template <typename Other>
	explicit constexpr inline Box(Box<Other>& other) : value_{other()} {
	}

	/// Copy constructor.
	template <typename Other>
	explicit constexpr inline Box(const Box<Other>& other) : value_{other()} {
	}

	/// Move constructor.
	template <typename Other>
	explicit constexpr inline Box(Box<Other>&& other) : value_{std::move(other())} {
	}

	~Box() = default;

	/// @group Accessors
	/// @{

	inline auto operator()() && -> Type&& {
		return value_;
	}

	inline auto operator()() & -> Type& {
		return value_;
	}

	inline constexpr auto operator()() const & -> const Type& {
		return value_;
	}

	/// @}

	template <typename Other> inline auto operator=(const Box<Other>& x) -> Box<Type>& {
		value_ = x();
		return *this;
	}

	template <typename Other> inline auto operator=(Box<Other>& x) -> Box<Type>& {
		value_ = std::move(x());
	}

	template <typename Other> inline auto operator=(const Other& x) -> Box<Type>& {
		value_ = x;
		return *this;
	}

	template <typename Other> inline auto operator=(Other&& x) -> Box<Type>& {
		value_ = std::move(x);
		return *this;
	}

protected:
	Type value_;
};

}  // namespace Pith

#endif  // PITH_BOX_HPP_
