#include <Om/Error.hpp>
#include <Om/Heap.hpp>
#include <Pith/Assert.hpp>
#include <Pith/Debug.hpp>
#include <Pith/SharedLock.hpp>
#include <system_error>

namespace Om {

auto Heap::init([[maybe_unused]] const HeapConfig& config) -> HeapError {
	/// TODO: Bring up the OMR heap here.
	PITH_TRACE();
	PITH_ASSERT(condition_ == HeapCondition::DEAD);
	auto e = lock_.init();
	PITH_ASSERT(e == Pith::SharedLockError::SUCCESS);
	condition_ = HeapCondition::ACTIVE;
	return HeapError::SUCCESS;
}

auto Heap::kill() -> HeapError {
	PITH_TRACE();
	PITH_ASSERT(condition_ == HeapCondition::ACTIVE);
	condition_ = HeapCondition::DEAD;
	auto e = lock_.kill();
	PITH_ASSERT(e == Pith::SharedLockError::SUCCESS);
	return HeapError::SUCCESS;
}

}  // namespace Om
