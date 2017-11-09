#ifndef OM_ALLOCATOR_INL_HPP_
#define OM_ALLOCATOR_INL_HPP_

#include <Om/Allocator.hpp>

namespace Om {

template <typename T>
Allocator<T>::Allocator(AllocatorContext& cx) : cx_{cx} {
}

template <typename T>
template <typename U>
Allocator<T>::Allocator(const Allocator<U>& a) : cx_{a.cx_} {
}

template <typename T>
Allocator<T>::~Allocator() noexcept {
}

template <typename T>
inline auto Allocator<T>::allocate(std::size_t n) -> Ref<T> {
	return nullptr;  // OMR_GC_Allocate(cx_->omr(), 0, size * sizeof(T), 0);
}

template <typename T>
inline auto Allocator<T>::deallocate(Ref<T> p) -> void {
	// do nothing!
}

template <typename T>
template <typename U, typename... Args>
auto Allocator<T>::construct(U* p, Args&&... args) -> void {
	return ::new (p) T(std::forward<Args>(args)...);
}

template <typename T>
auto AllocatorTraits<T>::allocate(Allocator<T>& a, std::size_t n) -> Ref<T> {
	return a.allocate(n);
}

template <typename T>
auto AllocatorTraits<T>::deallocate(Allocator<T>& a, Ref<T> ref, std::size_t n) -> void {
	a.deallocate(ref, n);
}

template <typename T>
template <typename U>
auto AllocatorTraits<T>::destroy(Allocator<T>& a, U* p) -> void {
	a.destroy(p);
}

// static
template <typename T>
template <typename U, typename... Args>
inline auto AllocatorTraits<T>::construct(Om::Allocator<T>& a, U* p, Args&&... args) -> void {
	a.construct(p, std::forward<Args>(args)...);
}

}  // namespace Om

#endif  // OM_ALLOCATOR_INL_HPP_
