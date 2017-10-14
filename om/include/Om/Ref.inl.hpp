#ifndef OM_REF_INL_HPP_
#define OM_REF_INL_HPP_

#include <Om/Ref.hpp>

namespace Om {

template <typename T>
constexpr Ref<T>::Ref() : value_{nullptr} {
}

template <typename T>
constexpr Ref<T>::Ref(std::nullptr_t) : Ref{} {
}

template <typename T>
template <typename U>
constexpr Ref<T>::Ref(U* other) : value_{other} {
}

template <typename T>
template <typename U>
constexpr Ref<T>::Ref(const Ref<U>& other) : value_{other.raw()} {
}

template <typename T>
constexpr auto Ref<T>::operator-> () const -> T* {
	return value_;
}

template <typename T>
constexpr auto Ref<T>::operator*() const -> T& {
	return *value_;
}

template <typename T>
constexpr auto Ref<T>::toAddress() const -> Pith::Address {
	return reinterpret_cast<Pith::Address>(value_);
}

template <typename T>
inline auto Ref<T>::operator=(std::nullptr_t rhs) -> Ref<T>& {
	value_ = nullptr;
	return *this;
}

template <typename T>
template <typename U>
inline auto Ref<T>::operator=(U* rhs) -> Ref<T>& {
	value_ = rhs;
	return *this;
}

template <typename T>
template <typename U>
inline auto Ref<T>::operator=(const Ref<U>& rhs) -> Ref<T>& {
	value_ = rhs.value_;
	return *this;
}

/// Cast Ref<A> to Ref<B>
template <typename T>
template <typename U>
constexpr auto Ref<T>::to() const -> Ref<U> {
	return Ref<U>{value_};
}

/// Obtain the raw pointer
template <typename T>
constexpr auto Ref<T>::raw() const -> T* {
	return value_;
}

template <typename T>
template <typename U>
constexpr auto Ref<T>::operator==(const Ref<U>& rhs) const -> bool {
	return value_ == rhs.value_;
}

template <typename T>
template <typename U>
constexpr auto Ref<T>::operator==(U* rhs) const -> bool {
	return value_ == rhs;
}

template <typename T>
constexpr auto Ref<T>::operator==(std::nullptr_t) const -> bool {
	return nullptr == toAddress();
}

template <typename T>
constexpr auto makeRef(T* p) -> Ref<T> {
	return Ref<T>(p);
}

template <typename T>
constexpr Ref<T>::operator Pith::Address() const {
	return toAddress();
}

template <typename T>
template <typename U>
constexpr Ref<T>::operator U*() const {
	return to<U>().raw();
}

}  // namespace Om

#endif  // OM_REF_INL_HPP_
