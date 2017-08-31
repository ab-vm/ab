#include <Ab/Config.hpp>
#include "Test.hpp"
#include <Ab/Instruction.hpp>
#include <Ab/Interpreter.hpp>
#include <gtest/gtest.h>

class TestInterpreter : public Ab::Test::BasicTest {};

using namespace Ab;

TEST_F(TestInterpreter, basic) {
	Interpreter interpreter;
	const Ab::Instruction program[] = {
		Ab::Instruction::UNREACHABLE,
	};
}
