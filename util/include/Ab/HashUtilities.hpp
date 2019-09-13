#ifndef AB_HASHUTILITIES_HPP_
#define AB_HASHUTILITIES_HPP_

#include <cstddef>
#include <functional>

namespace Ab {

/// A prime number seed that can be used as an initial hash for mixing.
///
constexpr std::size_t HASH_SEED = 17;

/// Mix two or more hashes together.
///
constexpr std::size_t mix(std::size_t a, std::size_t b) { return (a * 37) + b; }

template <typename... Ts>
constexpr std::size_t mix(std::size_t a, std::size_t b, Ts... xs) {
	return mix(mix(a, b), xs...);
}

/// Mix multiple hashes together with a seed to create a new hash.
///
template <typename... Ts>
constexpr std::size_t combine_hashes(Ts... xs) {
	return mix(HASH_SEED, xs...);
}

/// Fast hash implementation for 64bit std::size_t.
///
constexpr std::size_t hash(std::size_t x) {
	x ^= x >> 30;
	x *= 0xbf58476d1ce4e5b9;
	x ^= x >> 27;
	x *= 0x94d049bb133111eb;
	x ^= x >> 31;
	return x;
}

/// Default hash implementation: fall back to std library.
///
template <typename T>
constexpr std::size_t hash(T x) {
	return std::hash<T>()(x);
}

/// Hash each value, mixing the results together to produce a single hash.
///
template <typename... Ts>
constexpr std::size_t hash(Ts... xs) {
	return combine_hashes(Ab::hash(xs)...);
}

}  // namespace Ab

#endif  // AB_HASHUTILITIES_HPP_
