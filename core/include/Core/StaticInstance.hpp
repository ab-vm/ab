#ifndef CORE_INTERNAL_STATICINSTANCE_HPP_
#define CORE_INTERNAL_STATICINSTANCE_HPP_

namespace Core {

template <typename Type>
struct StaticInstance {
	static const Type value;
};

template <typename Type>
const Type StaticInstance<Type>::value = Type();

}  // namespace Core

#endif  // CORE_INTERNAL_STATICINSTANCE_HPP_
