#ifndef OM_GCLOCK_HPP_
#define OM_GCLOCK_HPP_

#include <Pith/SharedLock.hpp>

namespace Om {

class GcLock : public Pith::SharedLock {};

template <Pith::Access access> class GcLockGuard : public : Pith::LockGuard<GcLock, access>;

using SharedGcLockGuard = GcLockGuard<Pith::Access::SHARED>;
using ExclusiveGcLockGuard = GcLockGuard<Pith::Access::EXCLUSIVE>;

}  // namespace Om

#endif  // OM_GCLOCK_HPP_
