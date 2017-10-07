#ifndef OM_ALLOCATOR_INL_HPP_
#define OM_ALLOCATOR_INL_HPP_

#include <Om/Allocator.hpp>

namespace Om {

Allocator::Allocator() {
}

Allocator::~Allocator() {
}

auto Allocator::init() -> bool {
	return true;
}

auto Allocator::kill() -> bool {
	return true;
}

#if 0
template <typename InitFunction>
auto Allocator::allocate<GcSafe::YES, InitFunction>(ActiveContext& cx, InitFunction&& init)
	-> Pith::Maybe<Ref<Object>> {
	// TODO: Implement allocation
	PITH_ASSERT_UNREACHABLE();
	return Pith::NOTHING;
}

template <typename InitFunction>
auto Allocator::allocate<GcSafe::NO, InitFunction>(ActiveContext& cx, InitFunction&& init)
	-> Pith::Maybe<Ref<Object>> {
	// TODO: Implement allocation
	PITH_ASSERT_UNREACHABLE();
	return Pith::NOTHING;
}

template <GcSafe gcSafe>
auto Allocator::allocateRaw(std::size_t size) -> Pith::Address {
	return malloc(size);
}

#endif  // 0

template <GcSafe gcSafe, typename T, typename... Args>
auto Allocator::allocateNative(ActiveContext& acx, Args&&... args)
	-> Pith::Result<Ref<T>, AllocationError> {
	/// TODO: Implement OMR allocations
	PITH_TRACE();
	StackRootRef<T> root{acx, new T{args...}};
	return Pith::ok(root);
}

#if 0
template <typename T, typename... Args>
auto Allocator::allocateStruct(Args&&... args) -> Ref<StructCell<T>>
 {
		// TODO: Implement native allocation
		// TODO: Handle concurrency and locking
		allocate
		T* p = new T{std::forward<Args>(args)...};
		return Ref<StructCell<T>>{p};
}
#endif  // 0

}  // namespace Om

#endif  // OM_ALLOCATOR_INL_HPP_
