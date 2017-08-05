#if !defined(OM_LOCKGUARD_HPP_)
#define OM_LOCKGUARD_HPP_

#include "Om/Access.hpp"
#include "Om/SharedLock.hpp"

namespace Om {

template <typename LockType, Access access>
class LockGuard;

/// Readable alias to LockGuard<Lock, Access::exclusive>
template <typename LockType = SharedLock>
using SharedLockGuard = LockGuard<LockType, Access::shared>;

/// Readable alias to LockGuard<LockType, Access::shared>.
template <typename LockType = SharedLock>
using ExclusiveLockGuard = LockGuard<LockType, Access::exclusive>;

/// RAII: Holds shared access to lock for lifetime.
template <typename T>
class LockGuard<T, Access::shared> {
public:
	using LockType = T;

	/// Obtain shared access on the lock. Cannot fail, but will block.
	explicit LockGuard(LockType& lock);

	/// Release shared access to the SharedLock.
	~LockGuard<LockType, Access::shared>();

	/// Not copy asignable.
	LockGuard& operator=(const LockGuard& other) = delete;

	/// Yield shared access and block this thread;
	void yield();

protected:
	friend class LockGuard<LockType, Access::exclusive>;

	/// Release shared access. Unsafe.
	void disengage();

	/// Reaquire shared access. Unsafe.
	void engage();

private:
	LockType& lock_;
};

/// RAII: Holds a lock exclusively for lifetime.
template <typename T>
class LockGuard<T, Access::exclusive> {
public:
	using LockType = T;

	/// Obtain exclusive access. Cannot fail, but will block.
	explicit LockGuard(LockType& lock);

	/// release exclusive access
	~LockGuard();

	/// Not copy asignable.
	LockGuard& operator=(const LockGuard& other) = delete;

	/// Yield exclusive access and block this thread.
	void yield();

protected:
	friend class LockGuard<LockType, Access::shared>;

	/// Release exclusive access. Unsafe.
	void disengage();

	/// Reaquire exclusive access. Unsafe.
	void engage();

private:
	LockType& lock_;
};

/// TODO: implement
template <typename LockType>
inline SharedLockGuard<LockType> sharedLock(LockType& lock) {
	return SharedLockGuard<LockType>(lock);
}

/// TODO: implement
template <typename LockType>
inline ExclusiveLockGuard<LockType> exclusiveLock(LockType& lock) {
	return ExclusiveLockGuard<LockType>(lock);
}

/// Try to obtain shared access without blocking. Returns `nothing` on failure.
/// TODO: implement
template <typename LockType>
inline Maybe<SharedLockGuard<LockType>> trySharedLock(LockType& lock) {
	return SharedLockGuard<LockType>(lock);
}

/// Obtain exclusive access. Will not block. Can fail.
/// TODO: implement
template <typename LockType>
inline Maybe<ExclusiveLockGuard<LockType>> tryExclusiveLock(LockType& lock) {
	return ExclusiveLockGuard<LockType>(lock);
}

template <typename LockType>
LockGuard<LockType, Access::shared>::LockGuard(LockType& lock) : lock_{lock} {
	// lock_.lock<Access::shared>();
}

template <typename LockType>
LockGuard<LockType, Access::shared>::~LockGuard() {
	// lock_.unlock<Access::shared>();
}

template <typename LockType>
LockGuard<LockType, Access::exclusive>::LockGuard(LockType& lock) : lock_{lock} {
	// lock_.lock<Access::exclusive>();
}

template <typename LockType>
LockGuard<LockType, Access::exclusive>::~LockGuard() {
	// lock_.unlock<Access::exclusive>();
}

}  // namespace Om

#endif  // OM_LOCKGUARD_HPP_