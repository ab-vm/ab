#ifndef OM_ALLOCATOR_HPP_
#define OM_ALLOCATOR_HPP_

#include <Om/AllocationSite.hpp>
#include <Om/Bytes.hpp>
#include <Om/Cell.hpp>
#include <Om/Context.hpp>
#include <Om/Heap.hpp>
#include <Om/Maybe.hpp>
#include <Om/Object.hpp>
#include <Om/RawAllocator.hpp>

namespace Om {
namespace Mm {

struct ObjectTemplate {
	Size slotCount_;
};

struct AllocationRequest {
	ObjectTemplate objectTemplate_;
};

struct Allocation {
	Maybe<Object*> object_;
};

class MemoryContext {
	// TODO(rwy0717) nothing
};

class ObjectAllocator {
public:
	template <GcSafe gcSafe = GcSafe::yes>
	inline auto allocate(Context& cx, const AllocationRequest& site) -> Maybe<Ref<Object>> {
		auto alloc = Maybe<Ref<Object>>{rawAllocator_.allocate<gcSafe>(cx, mib(32))};
	}

	inline constexpr auto raw() -> RawAllocator& {
		return rawAllocator_;
	}

private:
	RawAllocator rawAllocator_;
};

}  // namespace Mm
}  // namespace Om

#endif  // OM_ALLOCATOR_HPP_