#ifndef OM_SYSTEM_LOCK_
#define OM_SYSTEM_LOCK_

#include <Om/Config.hpp>
#include <Pith/LockGuard.hpp>
#include <Pith/SharedLock.hpp>

namespace Om {

/// The system lock is a shared lock with unique typing.

class SystemLock : public Pith::SharedLock {};

template <Pith::Access access>
class SystemLockGuard : public Pith::LockGuard<SystemLock, access> {};

using SharedAccess = SystemLockGuard<Pith::Access::SHARED>;
using ExclusiveAccess = SystemLockGuard<Pith::Access::EXCLUSIVE>;

}  // namespace Om

#endif  // OM_SYSTEM_LOCK_
