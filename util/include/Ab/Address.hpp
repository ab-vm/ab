#ifndef AB_ADDRESS_HPP_
#define AB_ADDRESS_HPP_

#include <Ab/Config.hpp>
#include <Ab/Bytes.hpp>
#include <cstdint>
#include <type_traits>

namespace Ab {

using Address = const Byte*;

using MutAddress = Byte*;

constexpr MutAddress to_mut(Address address) { return const_cast<MutAddress>(address); }

/// Explicitly convert an address to a pointer.
///
template <typename T = const void>
constexpr T* to_ptr(Address address) {
	static_assert(std::is_const_v<T>);
	return reinterpret_cast<const T*>(address);
}

template <typename T = void>
constexpr T* to_ptr(MutAddress address) {
	return reinterpret_cast<T*>(address);
}

template <typename T = void>
constexpr T* to_mut_ptr(Address address) {
	return to_ptr<T>(to_mut(address));
}

template <typename T = void>
constexpr T* to_mut_ptr(MutAddress address) {
	return to_ptr<T>(address);
}

/// Explicitly convert a pointer to an address.
///
template <typename T>
constexpr Address to_address(T* ptr) {
	return reinterpret_cast<Address>(ptr);
}

template <typename T>
constexpr MutAddress to_mut_address(T* ptr) {
	static_assert(!std::is_const_v<T>);
	return reinterpret_cast<MutAddress>(ptr);
}

}  // namespace Ab

#endif  // AB_ADDRESS_HPP_
