#include <Ab/Wasm/Reader.hpp>
#include <Ab/Wasm/NoOpVisitor.hpp>
#include <gtest/gtest.h>

namespace Ab::Wasm::Test {

class EmptyModuleVisitor : public NoOpVisitor {};

TEST(TestReader, emptyModule) {
	NoOpVisitor visitor;
	Reader reader{visitor, std::cin};
}

}  // namespace Ab::Wasm::Test
