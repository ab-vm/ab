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

class CellAllocator {};

class AllocationCache {
public:
	auto allocate(std::size_t size) -> Pith::Address;
	auto clear() -> void;
	Pith::Address beg_;
	Pith::Address end_;
};

class AllocatorContext {
public:
	AllocationCache cache_;
};

class RawAllocator {
public:
	inline RawAllocator(AllocatorContext& cx);
};

/// An allocator for T
template <typename T>
class Allocator {
public:
	inline Allocator(AllocatorContext& cx);

	template <typename U>
	inline Allocator(const Allocator<U>& cx);

	inline ~Allocator() noexcept;

	auto allocate(std::size_t n) -> Ref<T>;

	auto deallocate(Ref<T> p) -> void;

	template <typename U, typename... Args>
	auto construct(U* p, Args&&... args) -> void;

	template <typename U>
	auto destroy(Ref<U> ref) -> void;

private:
	AllocatorContext& cx_;
};

/// std::allocator_traits for Allocator<T>.
/// This class is used by STL containers to interact with user defined containers.
/// see: http://en.cppreference.com/w/cpp/memory/allocator_traits
template <typename T>
class AllocatorTraits {
public:
	using allocator_type     = Allocator<T>;
	using value_type         = T;
	using pointer_type       = Ref<T>;
	using const_pointer_type = Ref<const T>;
	using void_pointer       = Ref<Cell>;
	using const_void_pointer = Ref<const Cell>;
	using difference_type    = std::ptrdiff_t;

	template <typename U>
	using rebind_alloc = Allocator<U>;

	/// Rebind the traits to a different typed allocator.
	template <typename U>
	using rebind_traits = AllocatorTraits<U>;

	/// Allocate objects.
	/// Does not construct objects.
	/// @param n the number of value_type objects.
	static auto allocate(Allocator<T>& a, std::size_t n) -> Ref<T>;

	/// @param the reference
	/// @param n the number of value_type (T) objects.
	static auto deallocate(Allocator<T>& a, Ref<T> ref, std::size_t n) -> void;

	/// Construct an object. Works on any type, at any address.
	template <typename U, typename... Args>
	static auto construct(Allocator<T>& a, U* p, Args&&... args) -> void;

	/// Destroy an object. Works on any type, at any address.
	template <typename U>
	static auto destroy(Allocator<T>& a, U* ref) -> void;
};

}  // namespace Om

namespace std {

template <typename T>
struct allocator_traits<Om::Allocator<T>> : public Om::AllocatorTraits<T> {};

}  // namespace std

#include <Om/Allocator.inl.hpp>

#endif  // OM_ALLOCATOR_HPP_
