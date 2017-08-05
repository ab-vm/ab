#ifndef OM_TYPETRAIT_NULLABLE
#define OM_TYPETRAIT_NULLABLE

#include <Om/Nothing.hpp>
#include <type_traits>

namespace Om {
namespace TypeTrait {

/// True if object can be assigned the nullptr_t type.
template <typename Type>
struct Nullable : public std::false_type {};
template <typename Type>
struct Nullable<Type*> : public std::true_type {};
// template <typename Type> struct Nullable<Ref<Type>> public std::true_type {};

template <typename Type>
struct Null : public std::integral_constant<std::nullptr_t, nullptr> {
	static_assert(Nullable<Type>::value, "Type is not Nullable");
};

}  // namespace TypeTrait
}  // namespace Om

#endif  // OM_TYPETRAIT_NULLABLE
