#ifndef AB_COMPILER_HPP_
#define AB_COMPILER_HPP_

#include <cstddef>
#include <cstdint>
#include <vector>
#include <memory>

/// ABX Bytecode Compiler

namespace Ab::XCompiler {

class CompilationBase {};

class SequencePoint {
};

/// Extended basic block.
struct ExtBasicBlock {
	std::vector<SequencePoint> sequence;
};

enum class BasicType {
	REF, I64, I32, F64, F32, PTR
};

struct ValueType {
	BasicType basic_;
};

union ValueConstData {
	std::uintptr_t asPtr;
};

/// A computed value.
class Value {
public:

private:
	std::size_t block_;
	std::shared_ptr<Node> node_;
};

class BasicBlock;

class User : public Value {};

class Phi : public Value {};

class Instruction : public User {
public:
	constexpr Op op() const { return op_; }

	constexpr BasicBlock* block() const { return block_; }

	constexpr void attachTo(BasicBlock* block) { block_ = block; }

	constexpr bool isUnary() const { return }
private:
	Op op_;
	BasicBlock* block_;
};

class UnaryInstruction : public Instruction {};

class BinaryInstruction : public Instruction {};

class MutVal;

// A lightweight handle to a computed intermediate value. The underlying 
using Val = std::shared_ptr<const Value>;

// A lightweight handle to a computed intermediate value.
using MutVal = std::shared_ptr<Value>;

struct Block {
	std::vector<std::shared_ptr<Expr>> seq;
};

Val mkval(Block& block, NodeHandle node) {

}

class ControlFlowGraph {

};

class Compilation : public CompilationBase {

};

enum class Op {
	I32_ADD, I32_SUB, I32_CONST
};

class Node {
public:
	virtual ~Node() = 0;

	constexpr Op op() const { return op_; }

protected:
	constexpr Node(Op op) : op_(op) {}

private:
	Op op_;
};

class I32ConstNode : public Node {
public:
	I32ConstNode(std::int32_t value) : Node(Op::I32_CONST), value_(value) {}

	std::int32_t value() const { return value_; }

private:
	std::int32_t value_;
};

inline Val i32_const(Block& b, std::int32_t v) {
	return mkval(b, new I32ConstNode(v));
}

class I32AddNode : public Node {
public:
	I32AddNode(Val lhs, Val rhs) : Node(Op::I32_ADD), lhs_(lhs), rhs_(rhs) {}

	virtual ~I32AddNode() override {}

	Val lhs() const { return lhs_; }

	Val rhs() const { return rhs_; }

private:
	Val lhs_;
	Val rhs_;
};

Val emitI32Add(Block& b, Val lhs, Val rhs) {
	return mkval(b, new I32AddNode(lhs, rhs));
}

class I32SubNode : public Node {
public:
	I32SubNode(Val lhs, Val rhs) : Node(Op::I32_SUB), lhs_(lhs), rhs_(rhs) {}

	virtual ~I32SubNode() override {}

	Val lhs() const { return lhs_; }

	Val rhs() const { return rhs_; }

private:
	Val lhs_;
	Val rhs_;
};

std::shared_ptr<Value> add(Block& c, const Value* lhs, const Value* rhs) {
	block.seq.push_back(std::make_shared<AddNode )
	return new std::make_shared<Value>(new Node())
}

Value* sub(Block& c, const Value* lhs, const Value* rhs) {
	return new Value()
}

Value* phi(Block& c, Value* a, Value* b) {
	return 
}

generate()

} // namespace Ab::Compiler

#endif // AB_COMPILER_HPP_
