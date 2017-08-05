#ifndef OM_INTERNAL_STATICINSTANCE_HPP_
#define OM_INTERNAL_STATICINSTANCE_HPP_

namespace Om {
namespace Internal {

template <typename Type>
struct StaticInstance {
	static const Type value;
};

template <typename Type>
const Type StaticInstance<Type>::value = Type();

}  // namespace Internal
}  // namespace Om

#endif  // OM_INTERNAL_STATICINSTANCE_HPP_
