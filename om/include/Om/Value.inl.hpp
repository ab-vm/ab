#ifndef OM_VALUE_INL_HPP_
#define OM_VALUE_INL_HPP_

#include <Om/Value.hpp>

namespace Om {

constexpr inline Value::Value() : value_{0} {
}

/// TODO: Assert Type is convertible to uintptr_t
template <typename Type>
constexpr inline Value::Value(const Type& x) : value_{(std::uintptr_t)x} {
	// static_assert(sizeof(Type) == sizeof(std::uintptr_t), "Values must be
	// pointer-width.");
}

/// TODO: Assert uintptr_t is convertible to Type.
template <typename Type>
constexpr inline auto Value::to() const -> Type {
	// static_assert(sizeof(Type) == sizeof(std::uintptr_t), "Values are pointer-width.");
	return static_cast<Type>(value_);
}

}  // namespace Om

#endif  // OM_VALUE_INL_HPP_