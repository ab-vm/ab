#ifndef PITH_SHAREDLOCK_HPP_
#define PITH_SHAREDLOCK_HPP_

#include <Pith/Config.hpp>
#include <Pith/Access.hpp>

namespace Pith {

enum class SharedLockError { SUCCESS = 0, FAIL };

/// A classic read-write lock.
/// SharedLockGuard sharedHeapAccess(heap.lock()); // obtain shared access to the heap
///   heap.allocate(sharedHeapAccess());
///   ExclusiveLockGuard exclusiveHeapAccess(heapLock); // upgrade
class SharedLock {
public:
	SharedLock() noexcept;

	~SharedLock() noexcept;

	auto init() noexcept -> SharedLockError;

	auto kill() noexcept -> SharedLockError;

	template <Access access> inline void lock() noexcept;

	template <Access access> inline auto unlock() noexcept -> void;

	template <Access access> inline constexpr auto isLocked() const noexcept -> bool;

private:
	int sharedCount_ = 0;
	int exclusiveCount_ = 0;
	// std::mutex mutex_;
	// atomic<int> sharedCount_;
};

///
/// Implementation
///

template <> inline auto SharedLock::lock<Access::SHARED>() noexcept -> void {
	// TODO: Implement
	sharedCount_ += 1;
};

template <> inline auto SharedLock::lock<Access::EXCLUSIVE>() noexcept -> void {
	// TODO: Implement
	exclusiveCount_ += 1;
}

template <> inline auto SharedLock::unlock<Access::SHARED>() noexcept -> void {
	// TODO: Implement
	sharedCount_ -= 1;
}

template <> inline auto SharedLock::unlock<Access::EXCLUSIVE>() noexcept -> void {
	// TODO: Implement
	exclusiveCount_ -= 1;
}

template <> inline constexpr auto SharedLock::isLocked<Access::SHARED>() const noexcept -> bool {
	// TODO: Implement
	return false;
}

template <> inline constexpr auto SharedLock::isLocked<Access::EXCLUSIVE>() const noexcept -> bool {
	// TODO: Implement
	return false;
}

}  // namespace Pith

#endif  // PITH_SHAREDLOCK_HPP_
