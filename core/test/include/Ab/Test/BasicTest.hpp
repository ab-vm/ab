#ifndef AB_TEST_BASICTEST_HPP_
#define AB_TEST_BASICTEST_HPP_

#include <Ab/Config.hpp>
#include <Ab/Runtime.hpp>
#include <gtest/gtest.h>

namespace Ab {
namespace Test {

/// Test Fixture providing global Ab Initialization.
/// Process-wide initialization. Not mt-safe.
class BasicTest : public ::testing::Test {
public:
	static auto SetUpTestCase() -> void {
		ASSERT_EQ(runtime.init(), 0);
		ASSERT_TRUE(runtime.initialized());
	}

	static auto TearDownTestCase() -> void {
		ASSERT_TRUE(runtime.initialized());
		runtime.kill();
		ASSERT_FALSE(runtime.initialized());
	}

	static Runtime runtime;
};

Runtime BasicTest::runtime;

}  // namespace Test
}  // namespace Ab

#endif  // AB_TEST_BASICTEST_HPP_
