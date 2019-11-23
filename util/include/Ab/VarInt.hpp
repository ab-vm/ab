#ifndef AB_VARUINT_HPP_
#define AB_VARUINT_HPP_

#include <Ab/Config.hpp>
#include <Ab/ByteBuffer.hpp>
#include <Ab/leb128.hpp>
#include <cstdint>

namespace Ab {

struct EncodingError : public std::runtime_error {
	using runtime_error::runtime_error;
};

/// Append a uleb128 encoded number to the buffer.
///
/// @tparam N the maximum number of bits the number allows.
///
template <std::size_t N, typename T>
inline void append_varuint(ByteBuffer& buffer, T value) {
	static_assert(std::is_integral_v<T>);
	static_assert(std::is_unsigned_v<T>);
	static_assert(N <= sizeof(T) * 8);

	constexpr std::uint8_t data_mask = 0b0111'1111;
	constexpr std::uint8_t flag_mask = 0b1000'0000;

	do {
		std::uint8_t byte = std::uint8_t(value) & data_mask;
		value = value >> 7;
		if (value != 0) {
			byte = byte | flag_mask;
		}
		buffer.append(byte);
	} while (value != 0);
}

/// Append a 7-bit number encoded in uleb128 to the buffer.
///
inline void append_varuint7(ByteBuffer& buffer, std::uint8_t value) {
	return append_varuint<7>(buffer, value);
}

/// Append a 32 bit number encoded in uleb128 to the buffer.
///
inline void append_varuint32(ByteBuffer& buffer, std::uint32_t value) {
	return append_varuint<32>(buffer, value);
}

}  // namespace Ab

#endif  // AB_VARUINT_HPP_
