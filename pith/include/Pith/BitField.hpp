#ifndef CORE_BITFIELD_HPP_
#define CORE_BITFIELD_HPP_

#include <Core/Value.hpp>

namespace Core {

template<typename T>
class BitField : public Value<T> {
public:

	// Invert bits.
	inline constexpr BitField complement() const {
		return ~this->value_;
	}

	// True if any bits are set.
	template <typename T>
	constexpr inline bool any(const T & bits) const {
		return this->value_ | bits;
	}

	// True if all bits are set.
	template <typename T>
	constexpr inline bool all(const T & bits) const {
		return (this->value_ | bits) == bits;
	}
};

}  // namespace Core

#endif  // OM_BITFIELD_HPP_
