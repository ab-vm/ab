#ifndef OM_ALLOCATOR_INL_HPP_
#define OM_ALLOCATOR_INL_HPP_

#include <Om/Allocator.hpp>

namespace Om {

template <typename T>
Allocator<T>::Allocator(AllocatorContext& cx) {
}

template <typename T>
inline auto Allocator<T>::allocate(std::size_t size, std::size_t alignment) -> Ref<T> {
	return nullptr;
}

template <typename T>
template <typename... Args>
auto Allocator<T>::construct(Ref<T> ref, Args&&... args) -> void {
	return ::new (ref.raw()) T(std::forward<Args>(args)...);
}

// static
template <typename T>
template <typename... Args>
inline auto AllocatorTraits<T>::construct(Om::Allocator<T>& a, Ref<T> ref, Args&&... args) -> void {
	a.construct(ref, std::forward<Args>(args)...);
}

}  // namespace Om

#endif  // OM_ALLOCATOR_INL_HPP_
