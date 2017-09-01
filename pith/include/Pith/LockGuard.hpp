#ifndef PITH_LOCKGUARD_HPP_
#define PITH_LOCKGUARD_HPP_

#include <Pith/Config.hpp>
#include <Pith/Access.hpp>
#include <Pith/Maybe.hpp>
#include <Pith/SharedLock.hpp>

namespace Pith {

template <typename LockType = SharedLock, Access access = Access::EXCLUSIVE>
class[[maybe_unused]] LockGuard;

/// Readable alias to LockGuard<Lock, Access::exclusive>
template <typename LockType = SharedLock>
using SharedLockGuard = LockGuard<LockType, Access::SHARED>;

/// Readable alias to LockGuard<LockType, Access::SHARED>.
template <typename LockType = SharedLock>
using ExclusiveLockGuard = LockGuard<LockType, Access::EXCLUSIVE>;

/// RAII: Holds shared access to lock for lifetime.
template <typename T> class LockGuard<T, Access::SHARED> {
public:
	using LockType = T;

	/// Obtain shared access on the lock. Cannot fail, but will block.
	explicit LockGuard(LockType& lock);

	/// Release shared access to the SharedLock.
	~LockGuard<LockType, Access::SHARED>();

	/// Not copy assignable.
	LockGuard& operator=(const LockGuard& other) = delete;

	/// Yield shared access and block the current thread.
	void yield();

protected:
	friend class LockGuard<LockType, Access::EXCLUSIVE>;

	/// Release shared access. Unsafe.
	void disengage();

	/// Reaquire shared access. Unsafe.
	void engage();

private:
	LockType& lock_;
};

/// RAII: Holds a lock exclusively for lifetime.
template <typename T> class LockGuard<T, Access::EXCLUSIVE> {
public:
	using LockType = T;

	/// Obtain exclusive access. Cannot fail, but will block.
	explicit LockGuard(LockType& lock);

	/// release exclusive access
	~LockGuard();

	/// Not copy assignable.
	LockGuard& operator=(const LockGuard& other) = delete;

	/// Yield exclusive access and block this thread.
	void yield();

protected:
	friend class LockGuard<LockType, Access::SHARED>;

	/// Release exclusive access. Unsafe.
	void disengage();

	/// Reaquire exclusive access. Unsafe.
	void engage();

private:
	LockType& lock_;
};

/// TODO: implement
template <typename LockType> inline SharedLockGuard<LockType> sharedLock(LockType& lock) {
	return SharedLockGuard<LockType>(lock);
}

/// TODO: implement
template <typename LockType> inline ExclusiveLockGuard<LockType> exclusiveLock(LockType& lock) {
	return ExclusiveLockGuard<LockType>(lock);
}

/// Try to obtain shared access without blocking. Returns `nothing` on failure.
/// TODO: implement
template <typename LockType> inline Maybe<SharedLockGuard<LockType>> trySharedLock(LockType& lock) {
	return SharedLockGuard<LockType>(lock);
}

/// Obtain exclusive access. Will not block. Can fail.
/// TODO: implement
template <typename LockType>
inline Maybe<ExclusiveLockGuard<LockType>> tryExclusiveLock(LockType& lock) {
	return ExclusiveLockGuard<LockType>(lock);
}

template <typename LockType>
LockGuard<LockType, Access::SHARED>::LockGuard(LockType& lock) : lock_{lock} {
	// lock_.lock<Access::shared>();
}

template <typename LockType> LockGuard<LockType, Access::SHARED>::~LockGuard() {
	// lock_.unlock<Access::shared>();
}

template <typename LockType>
LockGuard<LockType, Access::EXCLUSIVE>::LockGuard(LockType& lock) : lock_{lock} {
	// lock_.lock<Access::EXCLUSIVE>();
}

template <typename LockType> LockGuard<LockType, Access::EXCLUSIVE>::~LockGuard() {
	// lock_.unlock<Access::EXCLUSIVE>();
}

}  // namespace Pith

#endif  // PITH_LOCKGUARD_HPP_
