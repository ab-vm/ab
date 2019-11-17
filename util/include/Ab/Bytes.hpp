#ifndef AB_BYTES_HPP_
#define AB_BYTES_HPP_

#include <Ab/Config.hpp>

#include <cstdint>

namespace Ab {

using Byte = unsigned char;

template <typename T>
constexpr T bytes(const T n) noexcept {
	return n;
}

template <typename T>
constexpr T kibibytes(const T n) noexcept {
	return 1024 * n;
}

template <typename T>
constexpr T mebibytes(const T n) noexcept {
	return 1024 * kibibytes(n);
}

template <typename T>
constexpr T gibibytes(const T n) noexcept {
	return 1024 * mebibytes(n);
}

template <typename T>
constexpr T tebibytes(const T n) noexcept {
	return 1024 * gibibytes(n);
}

constexpr int popcount(unsigned int x) noexcept {
	return __builtin_popcount(x);
}

constexpr int popcount(unsigned long x) noexcept {
	return __builtin_popcountl(x);
}

constexpr int popcount(unsigned long long x) noexcept {
	return __builtin_popcountll(x);
}

/// Numeric memory literals
///
inline namespace ByteLiterals {

constexpr unsigned long long int  operator"" _B(unsigned long long int x) {
	return ::Ab::bytes(x);
}

constexpr unsigned long long int  operator"" _KiB(unsigned long long int x) {
	return ::Ab::kibibytes(x);
}

constexpr unsigned long long int  operator"" _MiB(unsigned long long int x) {
	return ::Ab::mebibytes(x);
}

constexpr unsigned long long int  operator"" _GiB(unsigned long long int x) {
	return ::Ab::gibibytes(x);
}

}  // namespace ByteLiterals
}  // namespace Ab

#endif  // AB_BYTES_HPP_
