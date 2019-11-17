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
	static void SetUpTestCase() {}

	static void TearDownTestCase() {}
};

}  // namespace Test
}  // namespace Ab

#endif  // AB_TEST_BASICTEST_HPP_
