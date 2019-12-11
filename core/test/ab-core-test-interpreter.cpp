#include <Ab/Config.hpp>
#include <Ab/FuncBuilder.hpp>
#include <Ab/Interpreter.hpp>
#include <Ab/Loading.hpp>
#include <Ab/ModuleBuilder.hpp>
#include <Ab/ModuleWriter.hpp>
#include <Ab/Test/BasicTest.hpp>
#include <Ab/Test/RuntimeEnv.hpp>
#include <Ab/VirtualMachine.hpp>
#include <gtest/gtest.h>

namespace Ab::Test {

TEST(TestTypes, Metaprogramming) {
	using ty = type_sequence<int, double, float>;
	EXPECT_TRUE((std::is_same_v<element_type_t<1, ty>, double>));

	EXPECT_EQ((sum_slots_for<std::make_index_sequence<0>, int, double>::value), 0);
	EXPECT_EQ((sum_slots_for<std::make_index_sequence<1>, int, double>::value), 1);
	EXPECT_EQ((sum_slots_for<std::make_index_sequence<2>, int, double>::value), 3);

	EXPECT_EQ((sum_slots_up_to<0, int, int, int, int>::value), 0);
	EXPECT_EQ((sum_slots_up_to<1, int, int, int, int>::value), 1);
	EXPECT_EQ((sum_slots_up_to<2, int, int, int, int>::value), 2);
	EXPECT_EQ((sum_slots_up_to<3, int, int, int, int>::value), 3);
	EXPECT_EQ((sum_slots_up_to<4, int, int, int, int>::value), 4);

	EXPECT_EQ((sum_slots_up_to<0, double, double, double, double>::value), 0);
	EXPECT_EQ((sum_slots_up_to<1, double, double, double, double>::value), 2);
	EXPECT_EQ((sum_slots_up_to<2, double, double, double, double>::value), 4);
	EXPECT_EQ((sum_slots_up_to<3, double, double, double, double>::value), 6);
	EXPECT_EQ((sum_slots_up_to<4, double, double, double, double>::value), 8);

	EXPECT_EQ((nth_slot_index<0, int, double>::value), 0);
	EXPECT_EQ((nth_slot_index<1, int, double>::value), 1);
	EXPECT_EQ((nth_slot_index<2, int, double>::value), 3);
}

class TestInterpreter : public BasicTest {};

TEST_F(TestInterpreter, InitializeVm) { VirtualMachine vm(runtime()); }

TEST_F(TestInterpreter, InitializeCx) {
	VirtualMachine vm(runtime());
	Context cx(&vm);
}

class ModuleBuilder {};

TEST_F(TestInterpreter, LoadEmptyModule) {
	VirtualMachine vm(runtime());
	Context cx(&vm);
	ModuleNode node;
	absl::Span<Byte> bytes         = node.write();
	std::shared_ptr<Module> module = compile(cx, bytes);
}

TEST_F(TestInterpreter, LoadTypeSection) {
	// build the module

	ModuleNode node;
	FuncType type;
	type.args.push_back(ValType::I32);
	type.rets.push_back(ValType::I32);
	node.types.push_back(type);

	// compile it

	VirtualMachine vm(runtime());
	Context cx(&vm);
	std::shared_ptr<Module> module = compile(cx, node.write());

	// check for the type.

	EXPECT_EQ(module->type_table().size(), 1);
	EXPECT_EQ(module->type_table()[0], type);
}

TEST_F(TestInterpreter, LoadAFunction) {
	// build the module

	ModuleNode module_node;

	module_node.types.push_back(FuncType());

	FuncNode func_node;
	func_node.type_idx = 0;
	func_node.nregs    = 0;
	func_node.insn_list.push_back(std::make_unique<HaltInsnNode>());
	module_node.funcs.push_back(std::move(func_node));

	// compile it

	VirtualMachine vm(runtime());
	Context cx(&vm);
	std::shared_ptr<Module> module = compile(cx, module_node.write());
}

TEST_F(TestInterpreter, InstantiateAFunction) {
	// build the module

	ModuleNode module_node;

	module_node.types.push_back(FuncType());

	FuncNode func_node;
	func_node.type_idx = 0;
	func_node.nregs    = 0;
	func_node.insn_list.push_back(std::make_unique<HaltInsnNode>());
	module_node.funcs.push_back(std::move(func_node));

	// compile it

	VirtualMachine vm(runtime());
	Context cx(&vm);
	ModuleInst* module = instantiate(cx, module_node.write());
}

TEST_F(TestInterpreter, InterpretAFunction) {
	// build the module

	ModuleNode module_node;

	module_node.types.push_back(FuncType{});

	FuncNode func_node;
	func_node.type_idx = 0;
	func_node.nregs    = 0;
	func_node.insn_list.push_back(std::make_unique<HaltInsnNode>());
	module_node.funcs.push_back(std::move(func_node));

	// compile it

	VirtualMachine vm(runtime());
	Context cx(&vm);
	ModuleInst* module = instantiate(cx, module_node.write());

	interpret(&cx.exec_state(), module->func_inst(0));
}

TEST_F(TestInterpreter, JustReturnNothing) {
	// build the module

	ModuleNode module_node;

	module_node.types.push_back(FuncType{{}, {}});

	FuncNode func_node;

	func_node.type_idx = 0;
	func_node.nregs    = 0;
	func_node.insn_list.push_back(std::make_unique<ReturnInsnNode>());
	module_node.funcs.push_back(std::move(func_node));

	// compile it

	VirtualMachine vm(runtime());
	Context cx(&vm);
	ModuleInst* module = instantiate(cx, module_node.write());

	static_call(cx, module->func_inst(0));
}

/// Test an identity function that returns it's i32 argument
///
TEST_F(TestInterpreter, I32IdentityFunc) {
	ModuleNode mod;
	push(mod.types, FuncType({ValType::I32}, {ValType::I32}));
	FuncNode& func_node = push(mod.funcs);
	func_node.type_idx  = 0;
	func_node.nregs     = 0;
	func_node.push<X32ReturnInsnNode>(0);

	VirtualMachine vm(runtime());
	Context cx(&vm);
	auto inst = instantiate(cx, mod.write());

	EXPECT_EQ(
		static_call<std::int32_t>(cx, inst->func_inst(0), std::int32_t(0)),
		std::make_tuple(0));

	EXPECT_EQ(
		static_call<std::int32_t>(cx, inst->func_inst(0), std::int32_t(42)),
		std::make_tuple(42));
}

/// Test a doubling function
///
TEST_F(TestInterpreter, I32DoubleFunc) {
	ModuleNode module_node;

	module_node.types.push_back(FuncType{{ValType::I32}, {ValType::I32}});

	FuncNode& func_node = push(module_node.funcs);
	func_node.type_idx  = 0;
	func_node.nregs     = 0;
	func_node.push<I32AddInsnNode>(0, 0, 0);
	func_node.push<X32ReturnInsnNode>(0);

	VirtualMachine vm(runtime());
	Context cx(&vm);
	ModuleInst* module = instantiate(cx, module_node.write());

	EXPECT_EQ(
		static_call<std::int32_t>(cx, module->func_inst(0), std::int32_t(3)),
		std::make_tuple(6));
}

/// Test a doubling function
///
TEST_F(TestInterpreter, I32AddFunc) {
	ModuleNode mod;

	mod.types.push_back(FuncType{{ValType::I32, ValType::I32}, {ValType::I32}});

	FuncNode& func = push(mod.funcs);
	func.type_idx  = 0;
	func.nregs     = 1;
	func.push<I32AddInsnNode>(2, 0, 1);
	func.push<X32ReturnInsnNode>(2);

	VirtualMachine vm(runtime());
	Context cx(&vm);
	ModuleInst* inst = instantiate(cx, mod.write());

	EXPECT_EQ(
		static_call<std::int32_t>(
			cx, inst->func_inst(0), std::int32_t(33), std::int32_t(44)),
		std::make_tuple(77));
}
#if 0

TEST(TestInterpreter, BranchOverNop) {
	const Func* fn = nullptr;

	{
		FuncBuilder fb;
		auto lbl = fb.mklabel();
		fb.emit_goto(lbl);
		fb.emit_nop();
		fb.place(lbl);
		fb.emit_halt();
		fn = fb.finalize();
	}

	Interpreter().interpretNil(fn);

	delete[] fn;
}

void my_primitive(Interpreter* interpreter) {
	fprintf(stderr, "my primitive was called\n");
};

TEST(TestInterpreter, CallPrimitive) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emit_call_primitive((uintptr_t)&my_primitive);
		fb.emit_halt();
		fn = fb.finalize();
	}

	Interpreter().interpretNil(fn);

	delete[] fn;
}

TEST(TestInterpreter, ReturnInt32) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emitI32Const(0xcafe);
		fb.emit_halt();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), 0xcafe);
	delete[] fn;
}

TEST(TestInterpreter, X32SelectTrue) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emitI32Const(4);
		fb.emitI32Const(5);
		fb.emitI32Const(1);
		fb.emitX32Select();
		fb.emit_halt();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), 4);
	delete[] fn;
}

TEST(TestInterpreter, X32SelectFalse) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emitI32Const(4);
		fb.emitI32Const(5);
		fb.emitI32Const(0);
		fb.emitX32Select();
		fb.emit_halt();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), 5);
	delete[] fn;
}

TEST(TestInterpreter, I32EqzTrue) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emitI32Const(0);
		fb.emitI32Eqz();
		fb.emit_halt();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), 1);
	delete[] fn;
}

TEST(TestInterpreter, I32EqzFalse) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emitI32Const(1);
		fb.emitI32Eqz();
		fb.emit_halt();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), 0);
	delete[] fn;
}

TEST(TestInterpreter, I32EqTrue) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emitI32Const(42);
		fb.emitI32Const(42);
		fb.emitI32Eq();
		fb.emit_halt();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), 1);
	delete[] fn;
}

TEST(TestInterpreter, I32EqFalse) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emitI32Const(42);
		fb.emitI32Const(41);
		fb.emitI32Eq();
		fb.emit_halt();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), 0);
	delete[] fn;
}

TEST(TestInterpreter, I32Add) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emitI32Const(4);
		fb.emitI32Const(6);
		fb.emitI32Add();
		fb.emit_halt();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), 10);
	delete[] fn;
}

TEST(TestInterpreter, I32Sub) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emitI32Const(4);
		fb.emitI32Const(6);
		fb.emitI32Sub();
		fb.emit_halt();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), -2);
	delete[] fn;
}

TEST(TestInterpreter, I32Shiftl) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emitI32Const(4);
		fb.emitI32Const(2);
		fb.emitI32Shl();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), 16);
	delete[] fn;
}

TEST(TestInterpreter, I32Shiftl_WrapShiftBitsTo0) {
	// shifting by greater than the data size will "wrap" or mod the shift by the data size.
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emitI32Const(4);
		fb.emitI32Const(32);
		fb.emitI32Shl();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), 4);
	delete[] fn;
}

TEST(TestInterpreter, I32Shiftl_WrapShiftBitsTo1) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		fb.emitI32Const(4);
		fb.emitI32Const(33);
		fb.emitI32Shl();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), 8);
	delete[] fn;
}

TEST(TestInterpreter, GotoIfTrue) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		auto label = fb.mklabel();
		fb.emitI32Const(1);
		fb.emit_gotoIf(label);
		fb.emit_nopConst(2);
		fb.emit_halt();
		fb.emit_halt();
		fb.emitI32Const(3);
		fb.emit_halt();
		fn = fb.finalize();
	}
	EXPECT_EQ(Interpreter().interpretInt32(fn), 3);
	delete[] fn;
}

TEST(TestInterpreter, ReturnArgument) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		auto param = fb.declX32Param();
		fb.emitX32GetLocal(param);
		fb.emit_halt();
		fn = fb.finalize();
	}
	Interpreter interpreter;
	interpreter.pushParam<std::int32_t>(42);
	EXPECT_EQ(interpreter.interpretInt32(fn), 42);
	delete[] fn;
}

TEST(TestInterpreter, AddToArgument) {
	const Func* fn = nullptr;
	{
		FuncBuilder fb;
		auto param = fb.declX32Param();
		fb.emitX32GetLocal(param);
		fb.emitI32Const(14);
		fb.emitI32Add();
		fb.emit_halt();
		fn = fb.finalize();
	}
	Interpreter interpreter;
	interpreter.pushParam<std::int32_t>(42);
	EXPECT_EQ(interpreter.interpretInt32(fn), 56);
	delete[] fn;
}

#endif  //////////////////////////////

}  // namespace Ab::Test
