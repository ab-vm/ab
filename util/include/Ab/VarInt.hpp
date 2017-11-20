#ifndef CORE_VARUINT_HPP_
#define CORE_VARUINT_HPP_

#include <Ab/Config.hpp>
#include <Ab/Leb128.hpp>
#include <cstdint>

namespace Core {

template <size_t n>
class VarInt {};
template <>
class VarInt<1> : public Leb128<std::int8_t> {};
template <>
class VarInt<7> : public Leb128<std::int8_t> {};
template <>
class VarInt<32> : public Leb128<std::int32_t> {};
template <>
class VarInt<64> : public Leb128<std::int64_t> {};

template <size_t n>
class UVarInt {};
template <>
class UVarInt<1> : public Leb128<std::uint8_t> {};
template <>
class UVarInt<7> : public Leb128<std::uint8_t> {};
template <>
class UVarInt<32> : public Leb128<std::uint32_t> {};
template <>
class UVarInt<64> : public Leb128<std::uint64_t> {};

}  // namespace Core

#endif  // CORE_VARUINT_HPP_
