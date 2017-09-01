#include <Ab/Config.hpp>
#include "Ab/Test/BasicTest.hpp"
#include <Ab/Instruction.hpp>
#include <Ab/Interpreter.hpp>
#include <gtest/gtest.h>

namespace Ab {
namespace Test {

class TestInterpreter : public BasicTest {};

TEST_F(TestInterpreter, basic) {
	[[maybe_unused]] const Ab::Instruction program[] = {
		Ab::Instruction::UNREACHABLE,
	};

	Interpreter interpret;
	InterpreterState state;

	interpret(state);
}

}  // namespace Test
}  // namespace Ab
