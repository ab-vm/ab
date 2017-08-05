#ifndef OM_SYSTEM_LOCK_
#define OM_SYSTEM_LOCK_

#include <Om/Config.hpp>
#include <Pith/SharedLock.hpp>
#include <Pith/LockGuard.hpp>

namespace Om {

/// The system lock is shared lock with unique typing.

class SystemLock : public Pith::SharedLock {};

template <Pith::Access access>
class SystemLockGuard : public Pith::LockGuard<SystemLock, access> {};

} // namespace Om

#endif // OM_SYSTEM_LOCK_
