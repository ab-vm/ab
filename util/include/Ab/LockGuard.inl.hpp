#ifndef AB_LOCKGUARD_INL_HPP_
#define AB_LOCKGUARD_INL_HPP_

#include <Ab/LockGuard.hpp>

namespace Ab {

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
LockGuard<LockType, Access::SHARED>::LockGuard(LockType& lock) : lock_{lock} {
	// lock_.lock<Access::shared>();
}

template <typename LockType>
LockGuard<LockType, Access::SHARED>::~LockGuard() {
	// lock_.unlock<Access::shared>();
}

template <typename LockType>
LockGuard<LockType, Access::EXCLUSIVE>::LockGuard(LockType& lock) : lock_{lock} {
	// lock_.lock<Access::EXCLUSIVE>();
}

template <typename LockType>
LockGuard<LockType, Access::EXCLUSIVE>::~LockGuard() {
	// lock_.unlock<Access::EXCLUSIVE>();
}

}  // namespace Ab

#endif  // AB_LOCKGUARD_INL_HPP_
