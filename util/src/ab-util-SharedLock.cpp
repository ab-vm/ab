#include <Ab/Config.hpp>
#include <Ab/Debug.hpp>
#include <Ab/SharedLock.hpp>

namespace Ab {

// TODO(rwy0717): Implement
SharedLock::SharedLock() noexcept = default;

// TODO(rwy0717): Implement
SharedLock::~SharedLock() noexcept = default;

auto SharedLock::init() noexcept -> SharedLockError {
	AB_TRACE();
	return SharedLockError::SUCCESS;
}

auto SharedLock::kill() noexcept -> SharedLockError {
	AB_TRACE();
	return SharedLockError::SUCCESS;
}

}  // namespace Ab
