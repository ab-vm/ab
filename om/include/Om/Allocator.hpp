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

class AllocatorContext {
public:
};

#if 0
class RawAllocator {
public:
	inline Allocator(AllocatorContext& cx) noexcept;

	inline ~Allocator();

	inline auto allocate(const std::size_t size, const std::size_t alignment) -> Ref<Cell>;

	inline auto allocate(
		const std::size_t size, const std::size_t alignment, const std::nothrow_t&) noexcept
		-> Ref<Cell>;

private:
};

#endif

template <typename T>
class Allocator {
public:
	Allocator(AllocatorContext& cx);

	auto allocate(std::size_t n, std::size_t alignment) -> Ref<T>;

	void deallocate(Ref<T> p);

	template <typename... Args>
	void construct(Ref<T> p, Args&&... args);
};

template <typename T>
struct AllocatorTraits {
	using type = T;
	using value_type = T;
	using pointer_type = Ref<T>;
	using const_pointer_type = Ref<const T>;

	template <typename A, typename U>
	struct rebind {
		using other = Allocator<U>;
	};

	template <class... Args>
	static void construct(Allocator<T>& a, Ref<T> p, Args&&... args);
};

}  // namespace Om

namespace std {

template <typename T>
struct allocator_traits<Om::Allocator<T>> : public Om::AllocatorTraits<T> {};

}  // namespace std

#include <Om/Allocator.inl.hpp>

#endif  // OM_ALLOCATOR_HPP_
