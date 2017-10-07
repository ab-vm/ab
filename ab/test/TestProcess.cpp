#include <Ab/Process.hpp>
#include <gtest/gtest.h>

namespace Ab {
namespace Test {

TEST(TestProcess, startAndStop) {
	/// NOTE: OMR doesn't support restarting (yet)
	for (int i = 0; i < 1; i++) {
		Process::init();
		Process::kill();
	}
}

}  // namespace Test
}  // namespace Ab
