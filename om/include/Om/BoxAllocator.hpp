#ifndef OM_BOXALLOCATOR_HPP_
#define OM_BOXALLOCATOR_HPP_

#include <Om/Config.hpp>
#include <Om/Allocator.hpp>
#include <Om/BoxCell.hpp>
#include <Om/BoxRef.hpp>

namespace Om {

class BoxAllocator {
public:
	t
};

template <typename T, GcSafe safe = GcSafe::YES>
class BoxAllocator {
public:
	BoxAllocator(AllocationContext& cx);

	auto allocate(std::size_t n) -> BoxRef<T>;

	auto deallocate(const BoxRef<T>& p, std::size_t n) -> void;

private:
	AllocationContext& cx_;
};

}  // namespace Om

namespace std {

template <typename T>
struct allocator_traits<Om::BoxAllocator<T>> {
	using pointer_type = BoxRef<T>;

	template <typename U>
	using rebind_alloc = BoxAllocator<U>;
};

}  // namespace std

#include <Om/BoxAllocator.inl.hpp>

#endif  // OM_DATAALLOCATOR_HPP_
