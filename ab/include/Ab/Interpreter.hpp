#ifndef AB_INTERPRETER_HPP_
#define AB_INTERPRETER_HPP_

#include <Ab/Config.hpp>
#include <Ab/Instruction.hpp>
#include <Pith/Assert.hpp>
#include <iostream>

namespace Ab {

struct InterpreterState {
public:
	Instruction* program_;
	uintptr_t* stack_;
	std::size_t pc_;
	std::uint8_t* stackPointer_;
	std::uint8_t* basePointer_;
	bool trap_;
};

struct Frame {
public:
	InterpreterState state_;
};

class Stack {};

class Interpreter {
public:
	auto inline operator()(InterpreterState& state) -> void;

private:
	auto next(InterpreterState& state) -> Instruction;
};

}  // namespace Ab

#include <Ab/Interpreter.inl.hpp>

#endif  // AB_INTERPRETER_HPP_
