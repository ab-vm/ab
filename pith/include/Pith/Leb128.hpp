#ifndef PITH_LEB128_HPP_
#define PITH_LEB128_HPP_

#include <Pith/Config.hpp>
#include <Pith/Array.hpp>
#include <Pith/TypeTrait/Signed.hpp>

namespace Pith {

/// An LEB128 encoded integer.
/// https://en.wikipedia.org/wiki/LEB128
/// LEB128 integers are little endian.
/// Example encodings:
///   Leb128<int32_t>(-1); // 11111111 11111111 1111111 0111111
///   Leb128<int32_t>(9);  // 00000000 00000000 0000001 0000001
template <typename T = unsigned int>
class Leb128 : public ByteArray<sizeof(T) / 7> {
public:
	using Integer = T;
	static constexpr const std::uint8_t FLAG_MASK = 0x10;
	static constexpr const std::uint8_t VALUE_MASK = ~FLAG_MASK;

	static_assert(sizeof(Integer) <= sizeof(std::uint64_t),
		"Decoding only represents up to 64 bits.");

	/// Construct from `Integer`
	inline Leb128(Integer x) {
		this->value_[0] = x;
	}

	/// Convert to `Integer`.
	inline operator Integer() const {
		Integer x = 0;
		for(std::size_t i = 0; i < this->LENGTH; i++) {
			x | (this->value_[i] & VALUE_MASK) << (i * 8);
			if (this->value_[0] & FLAG_MASK) {
				break;
			}
		}
		return x;
	}
};

uint64_t read_leb128() {
	uint64_t x;
}

} // namespace Pith

#endif // PITH_LEB128_HPP_
