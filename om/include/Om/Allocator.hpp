#ifndef OM_ALLOCATOR_HPP_
#define OM_ALLOCATOR_HPP_

#include <Om/Config.hpp>
#include <Om/GcSafe.hpp>
#include <Om/NativeCell.hpp>
#include <Om/Object.hpp>
#include <Om/Ref.hpp>
#include <Om/StackRootRef.hpp>
#include <Pith/Debug.hpp>
#include <Pith/Result.hpp>

namespace Om {

/// Allocates from a per-context cache aka TLH.
class CacheAllocator {};

class ActiveContext;

class AllocationContext {
public:
};

enum class AllocationError {
	OUT_OF_MEMORY,
};

class Allocator {
public:
	inline Allocator();

	inline ~Allocator();

	inline auto init() -> bool;

	inline auto kill() -> bool;

#if 0
	template <GcSafe gcSafe, typename InitFunction>
	inline auto allocate(InitFunction init) -> Pith::Maybe<Ref<Object>>;

	template <typename InitFunction>
	auto inline allocate<GcSafe::YES, InitFunction>(ActiveContext& cx, InitFunction&& init) -> Pith::Maybe<Ref<Object>>;

	template <typename InitFunction>
	auto inline allocate<GcSafe::NO, InitFunction>(ActiveContext& cx, InitFunction&& init) -> Pith::Maybe<Ref<Object>>;

#endif  // 0

	template <GcSafe gcSafe, typename T, typename... Args>
	inline auto allocateNative(ActiveContext& cx, Args&&... args)
		-> Pith::Result<Ref<T>, AllocationError>;

#if 0
	template <GcSafe gcSafe, typename T, typename... Args>
	auto inline allocateStruct(ActiveContext& cx, Args&&... args) -> Ref<StructCell<T>>;
#endif  // 0

private:
#if 0
	template <GcSafe gcSafe>
	auto inline allocateRaw(std::size_t size) -> Pith::Address;
#endif  // 0
};

}  // namespace Om

#include <Om/Allocator.inl.hpp>

#endif  // OM_ALLOCATOR_HPP_
