#ifndef OM_MM_RAWALLOCATOR_HPP_
#define OM_MM_RAWALLOCATOR_HPP_

#include <Om/Context.hpp>
#include <Om/Core.hpp>
#include <Om/Maybe.hpp>

namespace Om {
namespace Mm {

/// A byte oriented allocator. Allocates without other effects.
/// The innermost allocator. Every allocation must reduce to a contiguous set of bytes, allocated by
/// this type.
class RawAllocator {
	/// Raw byte-allocator
	template <GcSafe safe = GcSafe::yes> inline auto allocate(Context& cx, Size size) -> void*;
};

template <>
inline auto GcRawAllocator::allocate<GcSafe::yes>(Context& cx, Size size) -> Maybe<Address> {
	// TODO: Implement allocation
	return Maybe<Address>{static_cast<Address>(malloc(size))};
}

template <>
inline auto GcRawAllocator::allocate<GcSafe::no>(Context& cx, Size size) -> Maybe<Address> {
	// TODO: Implement allocation
	return Maybe<Address>{static_cast<Address>(malloc(size))};
}

}  // namespace Mm
}  // namespace Om

#endif  // OM_MM_RAWALLOCATOR_HPP_