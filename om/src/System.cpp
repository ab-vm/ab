#include <Om/Config.hpp>
#include <Om/Heap.hpp>
#include <Om/System.hpp>
#include <Om/SystemLock.hpp>
#include <Pith/Assert.hpp>

namespace Om {

constexpr const SystemConfig System::DEFAULT_CONFIG;

System::~System() {
	PITH_ASSERT(state_ == SystemState::DEAD);
}

auto System::init(const SystemConfig& config) -> SystemError {
	PITH_ASSERT(state_ == SystemState::DEAD);
	PITH_ASSERT(heap_.init(config.heap_) == HeapError::SUCCESS);
	PITH_ASSERT(lock_.init() == Pith::SharedLockError::SUCCESS);
	state_ = SystemState::ACTIVE;
	return SystemError::SUCCESS;
}

auto System::kill() -> SystemError {
	PITH_ASSERT(state_ == SystemState::ACTIVE);
	heap_.kill();
	lock_.kill();
	PITH_ASSERT(contexts_.empty());
	state_ = SystemState::DEAD;
	return SystemError::SUCCESS;
}

auto System::attach(Context* cx) -> AttachError {
	contexts_.insert(cx);
	return AttachError::SUCCESS;
}

auto System::detach(Context* cx) -> AttachError {
	contexts_.erase(cx);
	return AttachError::SUCCESS;
}

}  // namespace Om
