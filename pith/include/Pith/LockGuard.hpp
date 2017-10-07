#ifndef PITH_LOCKGUARD_HPP_
#define PITH_LOCKGUARD_HPP_

#include <Pith/Config.hpp>
#include <Pith/Access.hpp>
#include <Pith/Maybe.hpp>
#include <Pith/SharedLock.hpp>

namespace Pith {

/// RAII: Holds a lock for the duration of it's lifetime.
/// The LockGuard is capable of locking any subclass of SharedLock.
/// The LockGuard is parameterized on it's access.
///   * Access::SHARED: Multiple lockguards hold the SharedLock.
///   * Access::EXCLUSIVE: No lock guard
template <typename LockType = SharedLock, Access access = Access::EXCLUSIVE>
class[[gnu::unused]] LockGuard;

/// RAII: Holds shared access to lock for lifetime.
template <typename T>
class[[gnu::unused]] LockGuard<T, Access::SHARED> {
public:
	using LockType = T;

	/// Obtain shared access on the lock. Cannot fail, but will block.
	inline explicit LockGuard(LockType & lock);

	/// Release shared access to the SharedLock.
	inline ~LockGuard<LockType, Access::SHARED>();

	/// Not copy assignable.
	LockGuard& operator=(const LockGuard& other) = delete;

	/// Yield shared access and block the current thread.
	void yield();

protected:
	friend class LockGuard<LockType, Access::EXCLUSIVE>;

	/// Release shared access. Unsafe.
	inline void disengage();

	/// Reaquire shared access. Unsafe.
	inline void engage();

private:
	LockType& lock_;
};

/// RAII: Holds a lock exclusively for lifetime.
template <typename T>
class[[gnu::unused]] LockGuard<T, Access::EXCLUSIVE> {
public:
	using LockType = T;

	/// Obtain exclusive access. Cannot fail, but will block.
	explicit LockGuard(LockType & lock);

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

/// Readable alias to LockGuard<LockType, Access::exclusive>
template <typename LockType = SharedLock>
using SharedLockGuard = LockGuard<LockType, Access::SHARED>;

/// Readable alias to LockGuard<LockTypeType, Access::SHARED>.
template <typename LockType = SharedLock>
using ExclusiveLockGuard = LockGuard<LockType, Access::EXCLUSIVE>;

}  // namespace Pith

#include <Pith/LockGuard.inl.hpp>

#endif  // PITH_LOCKGUARD_HPP_
