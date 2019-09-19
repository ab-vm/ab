#ifndef AB_BYTES_HPP_
#define AB_BYTES_HPP_

#include <Ab/Config.hpp>

#include <cstdint>

namespace Ab {

using Byte = unsigned char;

template <typename T>
inline constexpr auto bytes(const T n) -> T {
	return n;
}

template <typename T>
inline constexpr auto kibibytes(const T n) -> T {
	return 1024 * n;
}

template <typename T>
inline constexpr auto mebibytes(const T n) -> T {
	return 1024 * kibibytes(n);
}

template <typename T>
inline constexpr auto gibibytes(const T n) -> T {
	return 1024 * mebibytes(n);
}

template <typename T>
inline constexpr auto tebibytes(const T n) -> T {
	return 1024 * gibibytes(n);
}

constexpr int popcount(unsigned int x) {
	return __builtin_popcount(x);
}

constexpr int popcount(unsigned long x) {
	return __builtin_popcountl(x);
}

constexpr int popcount(unsigned long long x) {
	return __builtin_popcountll(x);
}

/// Numeric memory literals
///
inline namespace ByteLiterals {

constexpr auto operator"" _B(const unsigned long long int x) -> unsigned long long int {
	return ::Ab::bytes(x);
}

constexpr auto operator"" _KiB(const unsigned long long int x) -> unsigned long long int {
	return ::Ab::kibibytes(x);
}

constexpr auto operator"" _MiB(const unsigned long long int x) -> unsigned long long int {
	return ::Ab::mebibytes(x);
}

constexpr auto operator"" _GiB(const unsigned long long int x) -> unsigned long long int {
	return ::Ab::gibibytes(x);
}

}  // namespace ByteLiterals

}  // namespace Ab

#endif  // AB_BYTES_HPP_
