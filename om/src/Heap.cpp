#include <Om/Error.hpp>
#include <Om/Heap.hpp>
#include <Pith/Assert.hpp>
#include <Pith/Debug.hpp>
#include <Pith/SharedLock.hpp>
#include <system_error>

namespace Om {

auto Heap::init(const HeapConfig& config) -> HeapError {
	PITH_TRACE();
	auto e = lock_.init();
	PITH_ASSERT(e == Pith::SharedLockError::SUCCESS);
	return HeapError::SUCCESS;
}

auto Heap::kill() -> HeapError {
	PITH_TRACE();
	auto e = lock_.kill();
	PITH_ASSERT(e == Pith::SharedLockError::SUCCESS);
	return HeapError::SUCCESS;
}

}  // namespace Om
