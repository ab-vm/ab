#ifndef AB_COMPILER_HPP_
#define AB_COMPILER_HPP_

#include <cstddef>
#include <cstdint>
#include <vector>
#include <memory>

/// ABX Bytecode Compiler

namespace Ab::Compiler {

class System {
public:
	System() {}

	System(const System&) = delete;

	System(System&&) = default;

	Context attach_thread() {
		return Context(this);
	}

private:
};

class Context {
public:
	friend class System;

	Context(Context&&) = default;

	Context(const Context&) = delete;

	Context& operator=(const Context&) = delete;

private:
	Context(System* system) {
		AB_ASSERT(system != nullptr);

	}

	System* system_;
};

class Function;


/**
 * A thing that can have metadata attached to it.
 */
class Entity {
public:
	Entity() = default;

	constexpr Entity(const Entity&) = default;

	constexpr explicit Entity(Id id) noexcept : id_(id) {}

	constexpr Id id() const noexcept { return id_; }

	Entity& operator=(const Entity&) noexcept = default;

private:
	Id id_;
};

class IdStore final {
public:
	Id alloc() noexcept { return Id(next_++); }

	void free([[maybe_unused]] Id id) noexcept {}

private:
	std::uint32_t next_;
};

class Operator {
protected:
	Operator()

private:

};

class Dialect {
public:
	Dialect(Context& cx, Id id) : id_(id) {}

private:
};

template <typename C>
class SparseStorage {
public:
	using ComponentType = C;

private:
	std::unordered_map<Id, C> data_;
};

class NameComponent {
public:
	NameComponent() {}

	NameComponent(std::string& name) : name_(name) {}

	NameComponent(std::string&& name) : name_(std::move(name)) {}

	const std::string& get() const { return name_; }

	void set(const std::string& name) { name_ = name; }

	void set(const std::string&& name) { name_ = std::move(name); }

private:
	std::string name_;
};

class NameStorage : public SparseStorage<NameComponent> {};

class NodeId : public EntityId {
public:
	NodeId() = default;

	explicit NodeId(std::uint32_t value) : value_(value) {}

	std::uint32_t get() const { return value_; }

private:
	std::uint32_t value_;
};

class DataArea {
};

class SequencePoint {
public:
	template <typename I, typename E>
	void append(I& iter, E& end) {
		for (; iter != end; ++iter) {
			push_back(*iter);
		}
	}

	void absorb(const SequencePoint& other) {
		append(other.nodes().begin(), other.nodes().end());
	}

	void push_back(Id node) { nodes_.push_back(node); }

	const std::vector<Id> nodes() const { return nodes_; }

private:
	std::vector<Id> nodes_;
};

inline SequencePoint merge(const SequencePoint& lhs, SequencePoint& rhs) {
	SequencePoint point;
	point.absorb(lhs);
	point.absorb(rhs);
	return point;
}

class Sequence {
public:
	leader() const {}

private:
	std::list<SequencePoint> points_;
};

class RefType {};

class Ref {
	RefType type_;
	NodeId target_;
	Id id_;
};

class BlockArg {
	Id id_;
	Ref ref_;
};

class BlockArgs {
	std::vector<BlockArg> arg_;
};

/**
 * A basic block.
 */
class Block : public Entity {
public:
	/**
	 * The first node in the block. The entry point.
	 */
	Id leader() const { return sequence_.leader(); }

	const BlockArgs& args() const noexcept { return args_; }

	BlockArgs& args() noexcept { return args_; }

private:
	BlockArgs args_;
	Node* entry_;
};

class Region : public Entity {
public:

};

class Superblock {
	BlockArgs args_;
	Node* entry_;
};

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

class Transformation {
public:
	virtual int operator()() {}

	~Transformation() = 0;
};

class NopTranformation : public Transformation {

};

class TransformationPipeline : public Transformation {

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

namespace core {

class Dialect : public abc::Dialect {
public:

};

class I32Const final : public Op {
public:
	I32ConstNode(std::int32_t value) : Node(Op::I32_CONST), value_(value) {}

	std::int32_t value() const { return value_; }

private:
	std::int32_t value_;
};

/**
 *
 */
class I32AddS : public Op {

};

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
