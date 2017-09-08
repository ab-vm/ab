#include <Pith/Config.hpp>
#include <Pith/Debug.hpp>
#include <Pith/SharedLock.hpp>

namespace Pith {

// TODO(rwy0717): Implement
SharedLock::SharedLock() noexcept = default;

// TODO(rwy0717): Implement
SharedLock::~SharedLock() noexcept = default;

auto SharedLock::init() -> SharedLockError {
	PITH_TRACE();
	return SharedLockError::SUCCESS;
}

auto SharedLock::kill() -> SharedLockError {
	PITH_TRACE();
	return SharedLockError::SUCCESS;
}

}  // namespace Pith
