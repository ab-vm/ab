#include <Ab/Config.hpp>
#include <Ab/FuncBuilder.hpp>
#include <Ab/Interpreter.hpp>
#include <Ab/ModuleBuilder.hpp>
#include <Ab/ModuleWriter.hpp>
#include <Ab/Test/BasicTest.hpp>
#include <Ab/Test/RuntimeEnv.hpp>
#include <Ab/VirtualMachine.hpp>
#include <gtest/gtest.h>

namespace Ab::Test {

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

	ModuleWriter module;

	// module_builder.finalize();
	// Module* module = module_builder.finalize();
	// link_module(cx, module);
	// delete module;
}

TEST_F(TestInterpreter, RunHaltFunction) {
	// FuncBuilder fb;
	// fb.emit_nop();
	// fb.emit_halt();

	// Func* f = fb.finalize();

	// ModuleBuilder mb;
	// auto id = mb.attach_function(f);
	// mb.export_function(id);

	// Module m = mb.finalize();
	// link_module(cx, m);
	// run_function(cx, f);
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
