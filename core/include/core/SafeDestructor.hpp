#ifndef CORE_SAFEDESTRUCTOR_HPP_
#define CORE_SAFEDESTRUCTOR_HPP_

#include <type_traits>

namespace Core {

/// Call `value`'s destructor, if `Type` is not trivially destructible.
/// Otherwise, do nothing.
template <typename Type, bool hasDestructor = !std::is_trivially_destructible<Type>::value>
struct SafeDestructor;

/// Has destructor
template <typename Type>
struct SafeDestructor<Type, true> {
	static inline auto destroy(Type& value) -> void {
		value.Type::~Type();
	}
};

/// No destructor
template <typename Type>
struct SafeDestructor<Type, false> {
	static inline auto destroy(Type& value) -> void {
		// Do nothing
	}
};

}  // namespace Core

#endif  // CORE_SAFEDESTRUCTOR_HPP_