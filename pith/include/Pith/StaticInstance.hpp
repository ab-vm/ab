#ifndef CORE_STATICINSTANCE_HPP_
#define CORE_STATICINSTANCE_HPP_

namespace Core {

/// Creates a single static instance of Type. Access the value using `StaticInstance<Type>::value`.
/// This relies on a C++ template trick to create a header only instance.
template <typename Type> struct StaticInstance { static const Type VALUE; };

template <typename Type> const Type StaticInstance<Type>::VALUE;

}  // namespace Core

#endif  // CORE_STATICINSTANCE_HPP_
