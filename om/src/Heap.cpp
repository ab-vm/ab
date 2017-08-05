#include <Om/Heap.hpp>
#include <Om/Error.hpp>
#include <Om/ErrorCategory.hpp>
#include <Pith/Assert.hpp>
#include <Pith/Debug.hpp>
#include <Pith/SharedLock.hpp>
#include <system_error>

namespace Om {

auto Heap::init(const HeapConfig & config) -> HeapError {
	PITH_TRACE();
	auto e = lock_.init();
	auto rc = e == Pith::SharedLockError::success ? 1 : 0;
	return HeapError::SUCCESS;
}

auto Heap::kill() -> HeapError {
	PITH_TRACE();
	auto e = lock_.kill();
	int rc = e == Pith::SharedLockError::success ? 1 : 0;
	return HeapError::SUCCESS;
}

}  // namespace Om
