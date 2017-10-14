#ifndef OM_BOXREF_INL_HPP_
#define OM_BOXREF_INL_HPP_

#include <Om/BoxRef.hpp>

namespace Om {

template <typename T>
constexpr auto BoxRef<T>::operator-> () const -> T* {
	return &value_->value();
}

template <typename T>
constexpr auto BoxRef<T>::operator*() const -> T& {
	return value_->value();
}

}  // namespace Om

#endif  // OM_BOXREF_INL_HPP_
