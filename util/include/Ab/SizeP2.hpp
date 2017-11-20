#if !defined(AB_SIZEP2_HPP_)
#define AB_SIZEP2_HPP_

#include <cstddef>
#include <cstdint>

namespace Ab {

/// An unsigned power of two.
///
class SizeP2 {
public:
	explicit constexpr SizeP2(std::uint8_t exponent) : exponent_(exponent) {}

	constexpr std::size_t get() const { return std::size_t(1) << exponent_; }

	constexpr std::size_t mod2() const { return exponent_; }

	constexpr std::size_t modmask() const { return get() - 1; }

	// raise value to the nth power.
	constexpr std::size_t pow(std::size_t n) const { return 1 << (exponent_ + n - 1); }

	explicit constexpr operator std::size_t() const { return get(); }

private:
	std::uint8_t exponent_;
};

constexpr std::size_t operator*(std::size_t lhs, SizeP2 rhs) {
	return lhs << rhs.mod2();
}

constexpr std::size_t operator/(std::size_t lhs, SizeP2 rhs) {
	return lhs >> rhs.mod2();
}

constexpr std::size_t operator%(std::size_t a, SizeP2 b) {
	return a & b.modmask();
}

constexpr SizeP2 SIZEP2_1   = SizeP2(0);
constexpr SizeP2 SIZEP2_2   = SizeP2(1);
constexpr SizeP2 SIZEP2_4   = SizeP2(2);
constexpr SizeP2 SIZEP2_8   = SizeP2(3);

constexpr SizeP2 SIZEP2_16  = SizeP2(4);
constexpr SizeP2 SIZEP2_32  = SizeP2(5);
constexpr SizeP2 SIZEP2_64  = SizeP2(6);

constexpr SizeP2 SIZEP2_128 = SizeP2(7);
constexpr SizeP2 SIZEP2_256 = SizeP2(8);
constexpr SizeP2 SIZEP2_512 = SizeP2(9);

}  // namespace Ab

#endif // AB_SIZEP2_HPP_
