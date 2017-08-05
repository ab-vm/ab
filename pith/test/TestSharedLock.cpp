#include <Pith/Config.hpp>
#include <Pith/LockGuard.hpp>
#include <Pith/SharedLock.hpp>
#include <gtest/gtest.h>

using namespace Pith;

TEST(SharedLock, multipleShared) {
	SharedLock lock;
	SharedLockGuard<SharedLock> shared1(lock);
	EXPECT_TRUE(trySharedLock(lock));
}

TEST(SharedLock, failToTakeExclusive) {
	SharedLock lock;
	SharedLockGuard<SharedLock> shared(lock);
	EXPECT_FALSE(tryExclusiveLock(lock));
}

TEST(SharedLock, exclusiveThenShared) {
	SharedLock lock;
	{
		ExclusiveLockGuard<SharedLock> exclusive(lock);
		EXPECT_TRUE(lock.isLocked<Access::EXCLUSIVE>());
	}
	EXPECT_FALSE(lock.isLocked<Access::EXCLUSIVE>());
	{
		SharedLockGuard<SharedLock> shared(lock);
		EXPECT_TRUE(lock.isLocked<Access::SHARED>());
	}
	EXPECT_FALSE(lock.isLocked<Access::SHARED>());
}