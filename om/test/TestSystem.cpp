#include <Om/System.hpp>
#include <gtest/gtest.h>

TEST() {
	SystemConfig config;
	config.minimumHeapSize_ = kib(1);
	config.maximumHeapSize_ = mib(100);
	System system(config);
	EXPECT_EQ(0, system.init());
	EXPECT_EQ(0, system.kill());
}
