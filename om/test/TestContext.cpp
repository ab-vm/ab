#include <Om/Config.hpp>
#include <Om/Context.hpp>
#include <gtest/gtest.h>

namespace Om {
namespace Test {

TEST(TestContext, basic) {
	System system;
	system.init();
	Context cx{system};
	cx.init();
	ActiveContext acx{context};
}

} // namespace Test
} // namespace Om
