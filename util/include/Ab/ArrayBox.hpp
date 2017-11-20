#ifndef AB_ARRAY_HPP_
#define AB_ARRAY_HPP_

#include <Ab/Config.hpp>
#include <Ab/Address.hpp>
#include <cstddef>

namespace Ab {

/// A simple struct wrapper around a fixed-size array.
template <typename T, std::size_t n>
class ArrayBox {
public:
	using Element = T;
	using Size    = std::size_t;

	static const constexpr std::size_t LENGTH = n;

	inline auto operator()() -> Element* {
		return value_;
	}

	inline constexpr auto operator()() const -> const Element* {
		return value_;
	}

	inline constexpr auto length() const -> std::size_t {
		return LENGTH;
	}

protected:
	Element value_[LENGTH];
};

template <std::size_t n>
using ByteArrayBox = ArrayBox<Byte, n>;

}  // namespace Ab

#endif  // AB_ARRAY_HPP_
