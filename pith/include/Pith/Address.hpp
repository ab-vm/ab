#ifndef PITH_ADDRESS_HPP_
#define PITH_ADDRESS_HPP_

#include <Pith/Config.hpp>
#include <cstdint>

namespace Pith {

using Byte = std::uint8_t;
using Address = const Byte*;

template <typename T = void>
inline constexpr T* toPointer(const Address address) {
	return (T*)(address);
}

}  // namespace Pith

#endif  // PITH_ADDRESS_HPP_
