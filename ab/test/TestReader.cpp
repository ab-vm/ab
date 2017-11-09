#include <Ab/Wasm/Binary/Reader.hpp>
#include <gtest/gtest.h>

namespace Ab {
namespace Wasm {
namespace Binary {
namespace Test {

class EmptyModuleVisitor : public NoOpVisitor {};

TEST(TestReader, emptyModule) {
	NoOpVisitor visitor;
	Reader reader{visitor, std::cin};
}

}  // namespace Test
}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab
