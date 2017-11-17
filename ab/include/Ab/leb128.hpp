#ifndef AB_LEB128_HPP_
#define AB_LEB128_HPP_

#include <cstddef>
#include <cstdint>
#include <istream>

namespace Ab {

class ReaderError : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

class BadNumber : public ReaderError {
public:
	BadNumber() : ReaderError{"Number too large or badly encoded"} {
	}
};

inline auto leb128(std::istream& in) -> std::int64_t {
	const std::uint8_t FLAG = 0b1000'0000;
	const std::uint8_t MASK = 0b0111'1111;
	const std::uint8_t SIGN = 0b0100'0000;

	std::uint64_t result = 0;
	std::size_t shift    = 0;
	std::uint8_t byte    = 0;

	do {
		if (shift > 63)
			throw BadNumber();

		byte = in.get();
		result |= std::uint64_t(byte & MASK) << shift;
		shift += 7;

	} while (byte & FLAG && (shift < 64) && !in.eof());

	if (byte & SIGN) {
		result |= ~0 << shift;
	}

	return result;
}

/// Read a uleb128 from a buffer. Reads up to n characters
inline auto uleb128(std::istream& in) -> std::uint64_t {
	constexpr std::uint8_t FLAG = 0b1000'0000;
	constexpr std::uint8_t MASK = 0b0111'1111;

	std::uint64_t result = 0;
	std::size_t shift    = 0;
	std::uint8_t byte    = 0;

	do {
		if (shift > 63)
			throw BadNumber();

		byte = in.get();
		result |= std::uint64_t(byte & MASK) << shift;
		shift += 7;
	} while ((byte & FLAG) != 0 && shift < 64 && !in.eof());

	return result;
}

inline auto varuint32(std::istream& in) -> std::uint64_t {
	return uleb128(in);
}

}  // namespace Ab

#endif  // AB_LEB128_HPP_
