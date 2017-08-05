#ifndef AB_WASM_BYTECODEVISITOR_HPP_
#define AB_WASM_BYTECODEVISITOR_HPP_

#include <Ab/Wasm/Bytecode.hpp>

namespace Ab {
namespace Wasm {

class BytecodeVisitor {
public:
	virtual void operator()(Wasm::Bytecode b);
};

class BytecodeVerifier : public BytecodeVisitor {
public:
	virtual void operator()(Wasm::Bytecode b);
};

} // namespace Wasm
} // namespace Ab

#endif // AB_WASM_BYTECODEVISITOR_HPP_
