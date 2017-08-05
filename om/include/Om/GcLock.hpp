#ifndef OM_GCLOCK_HPP_
#define OM_GCLOCK_HPP_

#include <Core/SharedLock.hpp>

namespace Om {

class GcLock : public Core::SharedLock {};

class SharedGcLockGuard : public Core::SharedLockGuard<GcLock> {};
class ExclusiveGcLockGuard : public Core::SharedLockGuard<GcLock> {};

}  // namespace Om

#endif  // OM_GCLOCK_HPP_
