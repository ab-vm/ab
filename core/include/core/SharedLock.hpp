#if !defined(OM_SHAREDLOCK_HPP_)
#define OM_SHAREDLOCK_HPP_

#include <Om/Access.hpp>
#include <Om/Maybe.hpp>
#include <system_error>

namespace Om {

/// A classic read-write lock.
/// SharedLockGuard sharedHeapAccess(heap.lock()); // obtain shared access to the heap
///   heap.allocate(sharedHeapAccess());
///   ExclusiveLockGuard exclusiveHeapAccess(heapLock); // upgrade
class SharedLock {
public:
	SharedLock();

	~SharedLock();

	std::error_condition init();

	std::error_condition kill();

	template <Access access>
	inline void lock();

	template <Access access>
	inline void unlock();

	template <Access access>
	inline bool isLocked();

private:
	int sharedCount_ = 0;
	int exclusiveCount_ = 0;
	// std::mutex mutex_;
	// atomic<int> sharedCount_;
};

///
/// Implementation
///

template <>
inline void SharedLock::lock<Access::shared>() {
	// TODO: Implement
	sharedCount_ += 1;
};

template <>
inline void SharedLock::lock<Access::exclusive>() {
	// TODO: Implement
	exclusiveCount_ += 1;
}

template <>
inline void SharedLock::unlock<Access::shared>() {
	// TODO: Implement
	sharedCount_ -= 1;
}

template <>
inline void SharedLock::unlock<Access::exclusive>() {
	// TODO: Implement
	exclusiveCount_ -= 1;
}

template <Access access>
inline bool SharedLock::isLocked() {
	// TODO: Implement
	return false;
}

}  // namespace Om

#endif  // OM_SHAREDLOCK_HPP_