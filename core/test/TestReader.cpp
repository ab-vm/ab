#include <Ab/Wasm/Reader.hpp>
#include <Ab/Wasm/NoOpVisitor.hpp>
#include <gtest/gtest.h>

namespace Ab {
namespace Wasm {

namespace Test {

class EmptyModuleVisitor : public NoOpVisitor {};

TEST(TestReader, emptyModule) {
	NoOpVisitor visitor;
	Reader reader{visitor, std::cin};
}

}  // namespace Test

}  // namespace Wasm
}  // namespace Ab
