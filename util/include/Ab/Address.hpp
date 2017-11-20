#ifndef AB_ADDRESS_HPP_
#define AB_ADDRESS_HPP_

#include <Ab/Config.hpp>
#include <Ab/Bytes.hpp>

#include <cstdint>

namespace Ab {

using Address = const Byte*;

using MutAddress = Byte*;

/// Explicitly convert an address to a pointer.
///
template <typename T = void>
constexpr T* to_ptr(Address address) {
	return reinterpret_cast<T*>(const_cast<MutAddress>(address));
}

/// Explicitly convert a pointer to an address.
///
template <typename T>
constexpr Address to_address(T* ptr) {
	return reinterpret_cast<Address>(ptr);
}

}  // namespace Ab

#endif  // AB_ADDRESS_HPP_
