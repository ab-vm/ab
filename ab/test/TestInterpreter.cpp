#include <Ab/Config.hpp>
#include <Ab/Interpreter.hpp>
#include <Ab/Instruction.hpp>
#include <gtest/gtest.h>
#include "Test.hpp"

class TestInterpreter : public Ab::Test::BasicTest {};

using namespace Ab;

TEST_F(TestInterpreter, basic) {
	Interpreter interpreter;
	const Ab::Instruction program[] = {
		Ab::Instruction::UNREACHABLE,
	};
}
