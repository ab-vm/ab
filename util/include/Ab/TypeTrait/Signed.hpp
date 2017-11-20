#ifndef AB_TYPETRAIT_SIGNED_HPP_
#define AB_TYPETRAIT_SIGNED_HPP_

#include <Ab/Config.hpp>

namespace Ab {
namespace TypeTrait {

/// Take an integer type and turn it into unsigned.
/// Usage:
///   typename Core::TypeTrait::MakeUnsigned<long>::Type i;
template <typename T>
struct Unsigned {
	using Type = void;
};

template <>
struct Unsigned<signed char> {
	using Type = unsigned char;
};
template <>
struct Unsigned<signed int> {
	using Type = unsigned int;
};
template <>
struct Unsigned<signed long> {
	using Type = unsigned long;
};
template <>
struct Unsigned<signed long long> {
	using Type = unsigned long long;
};

template <>
struct Unsigned<unsigned char> {
	using Type = unsigned char;
};
template <>
struct Unsigned<unsigned int> {
	using Type = unsigned int;
};
template <>
struct Unsigned<unsigned long> {
	using Type = unsigned long;
};
template <>
struct Unsigned<unsigned long long> {
	using Type = unsigned long long;
};

template <typename T>
struct IsSigned {};

template <>
struct IsSigned<signed char> {
	static constexpr const bool VALUE = true;
};
template <>
struct IsSigned<signed int> {
	static constexpr const bool VALUE = true;
};
template <>
struct IsSigned<signed long> {
	static constexpr const bool VALUE = true;
};
template <>
struct IsSigned<signed long long> {
	static constexpr const bool VALUE = true;
};

template <>
struct IsSigned<unsigned char> {
	static constexpr const bool VALUE = false;
};
template <>
struct IsSigned<unsigned int> {
	static constexpr const bool VALUE = false;
};
template <>
struct IsSigned<unsigned long> {
	static constexpr const bool VALUE = false;
};
template <>
struct IsSigned<unsigned long long> {
	static constexpr const bool VALUE = false;
};

}  // namespace TypeTrait
}  // namespace Ab

#endif  // AB_TYPETRAIT_SIGNED_HPP_
