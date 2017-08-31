#ifndef PITH_TYPETRAIT_NULLABLE_HPP_
#define PITH_TYPETRAIT_NULLABLE_HPP_

#include <Pith/Nothing.hpp>
#include <type_traits>

namespace Pith {
namespace TypeTrait {

/// True if object can be assigned the nullptr_t type.
/// By default, types are not nullable.
template <typename Type> struct Nullable : public std::false_type {};

template <typename Type> struct Null : public std::integral_constant<std::nullptr_t, nullptr> {
	static_assert(Nullable<Type>::value, "Type is not Nullable");
};

/// Pointer types are nullable.

template <typename Type> struct Nullable<Type*> : public std::true_type {};

template <typename Type>
struct Null<Type*> : public std::integral_constant<std::nullptr_t, nullptr> {};

}  // namespace TypeTrait
}  // namespace Pith

#endif  // PITH_TYPETRAIT_NULLABLE_HPP_
