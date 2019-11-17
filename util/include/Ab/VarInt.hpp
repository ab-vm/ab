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
	for (std::size_t shift = 0; shift < N; shift += 7) {
		buffer.append(static_cast<std::uint8_t>(value >> shift));
	}
	if (value & (std::numeric_limits<T>::max() << 7)) {
		throw EncodingError("Encoding error: number too large");
	}
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
