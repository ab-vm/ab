#include <Ab/Runtime.hpp>
#include <gtest/gtest.h>

namespace Ab {
namespace Test {

TEST(TestProcess, StartAndStop) {
	/// NOTE: OMR doesn't support restarting (yet)
	for (int i = 0; i < 1; i++) {
		ASSERT_NO_THROW({
			AutoRuntime runtime;
			EXPECT_TRUE(runtime.initialized());
		});

	}
}

}  // namespace Test
}  // namespace Ab
