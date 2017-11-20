#if !defined(AB_BYTECODEHANDLER_HPP_)
#define AB_BYTECODEHANDLER_HPP_

#include <Ab/Wasm/OpCode.hpp>

#include <IlBuilder.hpp>

namespace TR {}

namespace Ab {

struct InterpreterState {
	struct Offsets {
		static std::size_t instruction = offsetof(InterpreterState, instruction);
	};

	static void define(TR::TypeDictionary& d) {
	}

	std::uint8_t* instruction;
	std::uint8_t* memory;
	std::size_t memorySize;
	std::
};

static_assert(std::is_standard_layout_v<InterpreterState>);

struct OpcodeHandlerTable {
public:
	auto setHandler(OpCode code, void* handler) -> void {
	}
	auto getHandler(OpCode code) -> void* {
	}

private:
	void* table_[OpCode::COUNT];
};

class BaseHandlerBuilder : public TR::IlBuilder {
public:
	Dispatch() {
		TR::IlValue* selectorValue = Load(selectionVar);
		TR_ASSERT(
			selectorValue->getDataType() == TR::Int32,
			"Switch only supports selector having type Int32");
		*defaultBuilder = createBuilderIfNeeded(*defaultBuilder);

		TR::Node* defaultNode =
			TR::Node::createCase(0, (*defaultBuilder)->getEntry()->getEntry());
		TR::Node* lookupNode = TR::Node::create(
			TR::lookup, numCases + 2, loadValue(selectorValue), defaultNode);

		// get the lookup tree into this builder, even though we haven't completely filled
		// it in yet
		genTreeTop(lookupNode);
		TR::Block* switchBlock = _currentBlock;

		// make sure no fall through edge created from the lookup
		appendNoFallThroughBlock();

		TR::IlBuilder* breakBuilder = OrphanBuilder();

		// each case handler is a sequence of two builder objects: first the one passed in
		// via caseBuilder (or will be passed
		//   back via caseBuilders, and second a builder that branches to the breakBuilder
		//   (unless this case falls through)
		for (int32_t c = 0; c < numCases; c++) {
			int32_t value          = caseValues[c];
			TR::IlBuilder* handler = NULL;
			if (!caseFallsThrough[c]) {
				handler = OrphanBuilder();

				caseBuilders[c] = createBuilderIfNeeded(caseBuilders[c]);
				handler->AppendBuilder(caseBuilders[c]);

				// handle "break" with a separate builder so user can add whatever
				// they want into caseBuilders[c]
				TR::IlBuilder* branchToBreak = OrphanBuilder();
				branchToBreak->Goto(&breakBuilder);
				handler->AppendBuilder(branchToBreak);
			} else {
				caseBuilders[c] = createBuilderIfNeeded(caseBuilders[c]);
				handler         = caseBuilders[c];
			}

			TR::Block* caseBlock = handler->getEntry();
			cfg()->addEdge(switchBlock, caseBlock);
			AppendBuilder(handler);

			TR::Node* caseNode = TR::Node::createCase(0, caseBlock->getEntry(), value);
			lookupNode->setAndIncChild(c + 2, caseNode);
		}

		cfg()->addEdge(switchBlock, (*defaultBuilder)->getEntry());
		AppendBuilder(*defaultBuilder);

		AppendBuilder(breakBuilder);
	}
}
};  // namespace Ab

template <typename... Instruction>
class DispatchBuilder {};

template <>
class HandlerBuilder<OpCode::NOP> {
	void buildIL() {
		Dispatch();
	}
};

class InterpreterBuilder : public TR::MethodBuilder {
public:
	InterpreterBuilder(TR::TypeDictionary* types) : TR::MethodBuilder(types) {
	}

	bool buildIl() {
	}

	/// Read the opcode out of the VM state.
	TR::IlValue* Decode() {
		return state.instructionPointer->op();
	}

	/// Table of bytecodes
	TR::IlValue* Dispatch() {
		template <typename T... TR::Node* defaultNode = TR::Node::createCase()
	}
};

class HandlerBuilder : public TR::IlBuilder {
	HandlerBuilder(InterpreterBuilder* interpreterBuilder) : TR::IlBuilder()
};

}  // namespace Ab

#endif  // AB_BYTECODEHANDLER_HPP_

namespace TR {

class InterpreterBuilder {
	BuildIl(){

	};

	template <typename BuilderT, typename... Args>
	BuildBytecodeIL(BuilderT& b, Args&&... args) {
		il = b.BuildIL(args);
	}
};

}  // namespace TR

class ExecutionContextBuilder : public VirtualMachineState {
public:
	void push(TR::IlValue* value) {
	}

	TR::IlValue* pop(value) {
	}

	void call(TR::IlValue* target) {
	}

	/// VirtualMachineState

	// virtual Commit
	// virtual Merge
	// virtual Reload
	// virtual Clone
};

namespace b9 {

class AllocBuilder {

};
ValueBuilder(b).getDouble(v);

class Builder 
namespace Om {
namespace JitBuilder {


}
}

class State {};

class InterpretState : public State {
public:
	push / pop
};

class AddBytecode : public TR::BytecodeBuilder {
public:
	BuildIL(ExecutionContextBuilder& cx) {
		TR::IlValue* a = state->pop();
		TR::IlValue* b = state->pop();
		TR::IlValue* c = Add(a, b);
		state->push(c);
		state->dispatch(decode());
	};
};

class VmState {
	virtual Jump() {
		
	}
};

class JumpTrueBytecode : public TR::BytecodeBuilder {
public:
	BuildIl() {
		RE::IlValue* x = state->pop();
		TR::IlValue* p = Compare(ConstInt64(1), x);
		JumpIfEq(p, state->ip()->immediate());
	}
};

struct HandlerDef {
	std::size
};

B9InterpreterBuilder : public TR::InterpreterBuilder {
public:
	TR::IlValue* Decode() {
	}

	Intialize() {
		for (auto i = 0u; i < tableSize; i++) {
			table[i] = compileHandler(handlers[i]);
		}
	}

	Handler* compileHandler(BytecodeBuilder & b){b->};

	SetUpTable(std::size_t size) {
		table = malloc sizeof(Handler*) * size;
	}

	SetHandler(std::size_t index, Handler * h) {
		table[index] = h;
	}

	BuildIl() {
		TR::IlValue* index = Decode();
		RegisterBytecode()
	}

	Dispatch() {
		TR

			Table* table;
	}
