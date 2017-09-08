#include <Om/Config.hpp>
#include <Om/Context.hpp>
#include <Om/ActiveContext.hpp>
#include <gtest/gtest.h>

namespace Om {
namespace Test {

TEST(TestContext, basic) {
	System system;
	system.init(System::defaultConfig());
	Context cx{system};
	cx.init();
	{
		ActiveContext acx{cx};
	}
	cx.kill();
	system.kill();
}

} // namespace Test
} // namespace Om
