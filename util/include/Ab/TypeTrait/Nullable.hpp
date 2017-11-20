#ifndef AB_TYPETRAIT_NULLABLE_HPP_
#define AB_TYPETRAIT_NULLABLE_HPP_

#include <Ab/Nothing.hpp>
#include <type_traits>

namespace Ab {
namespace TypeTrait {

/// True if object can be assigned the nullptr_t type.
/// By default, types are not nullable.
template <typename Type>
struct Nullable : public std::false_type {};

template <typename Type>
struct Null : public std::integral_constant<std::nullptr_t, nullptr> {
	static_assert(Nullable<Type>::value, "Type is not Nullable");
};

/// Pointer types are nullable.

template <typename Type>
struct Nullable<Type*> : public std::true_type {};

template <typename Type>
struct Null<Type*> : public std::integral_constant<std::nullptr_t, nullptr> {};

}  // namespace TypeTrait
}  // namespace Ab

#endif  // AB_TYPETRAIT_NULLABLE_HPP_
