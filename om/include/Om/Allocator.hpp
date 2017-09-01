#ifndef OM_ALLOCATOR_HPP_
#define OM_ALLOCATOR_HPP_

#include <Om/Config.hpp>
#include <Om/GcSafe.hpp>
#include <Om/Object.hpp>
#include <Pith/Maybe.hpp>

namespace Om {

/// Allocates from a per-context cache aka TLH.
class CacheAllocator {};

class ActiveContext;

class AllocationContext {
public:
	Omr::TLH tlh_;
};

class Allocator {
public:
	Allocator() {
	}

	~Allocator() {
	}

	bool init() {
	}

	bool kill() {
	}

	template <GcSafe gcSafe, typename InitFunction>
	auto allocate(InitFunction init) -> Pith::Maybe<Ref<Object>>;

	template <typename InitFunction>
	auto inline allocate<GcSafe::yes, InitFunction>(ActiveContext& cx, InitFunction&& init)
		-> Pith::Maybe<Ref<Object>> {
		// TODO: Implement allocation
		PITH_ASSERT_UNREACHABLE();
		return Pith::NOTHING;
	}

	template <typename InitFunction>
	auto inline allocate<GcSafe::no, InitFunction>(ActiveContext& cx, InitFunction&& init)
		-> Pith::Maybe<Ref<Object>> {
		// TODO: Implement allocation
		PITH_ASSERT_UNREACHABLE();
		return Pith::NOTHING;
	}

	template <typename Native, typename ...Args>
	auto inline allocateNative(Args&&... args) -> Ref<Native> {
		// TODO: Implement native allocation
		// TODO: Handle concurrency and locking
		Data* p = new Native{std::forward<Args>(args)...};
		return Ref<Native>{p};
	}
private:
};

}  // namespace Om

#endif  // OM_ALLOCATOR_HPP_
