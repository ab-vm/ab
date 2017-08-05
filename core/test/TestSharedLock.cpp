#include "Om/LockGuard.hpp"
#include "Om/SharedLock.hpp"
#include <gtest/gtest.h>

using namespace Om;

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
		EXPECT_TRUE(lock.isLocked<Access::exclusive>());
	}
	EXPECT_FALSE(lock.isLocked<Access::exclusive>());
	{
		SharedLockGuard<SharedLock> shared(lock);
		EXPECT_TRUE(lock.isLocked<Access::shared>());
	}
	EXPECT_FALSE(lock.isLocked<Access::shared>());
}