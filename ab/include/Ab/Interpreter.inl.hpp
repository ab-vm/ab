#ifndef AB_INTERPRETER_INL_HPP_
#define AB_INTERPRETER_INL_HPP_

#include <Ab/Interpreter.hpp>

namespace Ab {

auto Interpreter::next(InterpreterState& state) -> Instruction {
	return state.program_[state.pc_];
}

auto Interpreter::operator()(InterpreterState& state) -> void {
	auto instruction = next(state);
	switch (instruction) {
	case Instruction::UNREACHABLE:
		PITH_ASSERT_UNREACHABLE();
		break;
	case Instruction::NOP:
		break;
	};
	state.pc_ += 1;
}

}  // namespace Ab

#endif  // AB_INTERPRETER_INL_HPP_
