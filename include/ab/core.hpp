#ifndef AB_CORE_HPP_
#define AB_CORE_HPP_

#include <cstdint>

namespace ab {

int ab_test();

#if 0

typedef unsigned long Value;

class Function {
public:
	uint32_t argumentCount;
	uint32_t variableCount;
	Instruction instructions[0];
};

class Frame {
public:
	Instruction* previousInstructionPointer;
	Value* previousDataPointer;
};

class ExecutionContext {
public:
	Instruction* instruction;
	Value* data;
	Frame* frame;
};

} // namespace ab

/*

  Instruction Language
*/

class Local;

class LocalSet {
public:
	uint32_t count = 0;

private:
	// friend Local;
};

class Local {
public:
	Local(LocalSet& lset) {
	}
};

namespace bc {
class Add {
	Add(Local& a, local& b, local& c) {
	}
};

} // namespace bc
#endif // 0

} // namespace ab

#endif // AB_CORE_HPP_