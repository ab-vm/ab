#include <Om/GcLock.hpp>
#include <gtest/gtest.h>

TEST(TestGcLock, basic) {
	SystemLock lock;
	SharedAccess shared1{lock};
	SharedAccess shared2{lock};
}
