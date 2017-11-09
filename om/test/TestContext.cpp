#include <Om/Config.hpp>
#include <Om/ActiveContext.hpp>
#include <Om/Context.hpp>
#include <gtest/gtest.h>

namespace Om {
namespace Test {

TEST(TestContext, basic) {
	System system{SystemConfig{}};
	Context cx{system};
	{ ActiveContext acx{cx}; }
}

}  // namespace Test
}  // namespace Om
