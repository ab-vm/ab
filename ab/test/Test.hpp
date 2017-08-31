#ifndef AB_TEST_BASICTEST_HPP_
#define AB_TEST_BASICTEST_HPP_

#include <Ab/Config.hpp>
#include <Ab/Process.hpp>
#include <gtest/gtest.h>

namespace Ab {
namespace Test {

/// Test Fixture providing global Ab Initialization.
/// Process-wide initialization. Not mt-safe.
class BasicTest : public ::testing::Test {
public:
	static auto SetUpTestCase() -> void {
		Ab::Process::init();
	}

	static auto TearDownTestCase() -> void {
		Ab::Process::kill();
	}
};

}  // namespace Test
}  // namespace Ab

#endif  // AB_TEST_BASICTEST_HPP_
