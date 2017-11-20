#ifndef AB_SAFEDESTRUCTOR_HPP_
#define AB_SAFEDESTRUCTOR_HPP_

#include <type_traits>

namespace Ab {

/// Call `value`'s destructor, if `Type` is not trivially destructible.
/// Otherwise, do nothing.
template <typename Type, bool hasDestructor = !std::is_trivially_destructible<Type>::value>
struct SafeDestructor;

/// Has destructor.
template <typename Type>
struct SafeDestructor<Type, true> {
	static inline auto destroy(Type& value) -> void {
		value.Type::~Type();
	}

	inline auto operator()(Type& value) -> void {
		destroy(value);
	}
};

/// No destructor.
template <typename Type>
struct SafeDestructor<Type, false> {
	static inline auto destroy([[gnu::unused]] Type& value) -> void {
		// Do nothing
	}

	inline auto operator()(Type& value) -> void {
		destroy(value);
	}
};

}  // namespace Ab

#endif  // AB_SAFEDESTRUCTOR_HPP_
