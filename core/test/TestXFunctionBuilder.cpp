#include <gtest/gtest.h>

#include <Ab/XExpr.hpp>
#include <Ab/XExprDisasm.hpp>
#include <Ab/XFunctionBuilder.hpp>
#include <Ab/XOpCode.hpp>

namespace Ab {

#if 0

template <typename T>
T& interpret(Byte* buffer, std::size_t offset) {
	return *reinterpret_cast<T*>(buffer + offset);
}

template <typename T>
testing::AssertionResult equalAt(Byte* buffer, std::size_t offset, const T& expected) {
	const auto& value = interpret<const T>(buffer, offset);
	if (value == expected) {
		return testing::AssertionSuccess() << buffer << "[" << offset << "]"
						   << "=" << value;
	}
	return testing::AssertionFailure() << buffer << "[" << offset << "]"
					   << "=" << value;
}

#define EXPECT_AT(buffer, offset, value) EXPECT_TRUE(equalAt(buffer, offset, value))

void printProgram(FILE* file, const Byte* program, std::size_t nbytes) {
	for (std::size_t i = 0; i < nbytes; ++i) {
		fprintf(file, "%p[%lu]=%u\n", program, i, uint(program[i]));
	}
}

TEST(TestXFunctionBuilder, EmptyFunction) {
	XFunctionBuilder fb;
	fb.emitNop();
	fb.emitNop();
	fb.emitNop();
	fb.emitNop();
	fb.emitHalt();

	auto fn = fb.finalize();

	EXPECT_EQ(fn->header.paramBytes, 0);
	EXPECT_EQ(fn->header.localBytes, 0);

	EXPECT_AT(fn->body, 0, XOpCode::NOP);
	EXPECT_AT(fn->body, 1, XOpCode::NOP);
	EXPECT_AT(fn->body, 2, XOpCode::NOP);
	EXPECT_AT(fn->body, 3, XOpCode::NOP);
	EXPECT_AT(fn->body, 4, XOpCode::HALT);
}

TEST(TestXFunctionBuilder, GotoSamePc) {
	XFunctionBuilder fb;
	auto label = fb.mklabel();
	fb.place(label);
	fb.emitGoto(label);

	auto fn = fb.finalize();

	EXPECT_AT(fn->body, 0, XOpCode::GOTO);
	EXPECT_AT(fn->body, 1, std::int32_t(-5));

	delete fn;
}

TEST(TestXFunctionBuilder, GotoForward0) {
	XFunctionBuilder fb;
	auto label = fb.mklabel();
	fb.emitGoto(label);
	fb.place(label);
	fb.emitNop();

	auto fn = fb.finalize();

	EXPECT_AT(fn->body, 0, XOpCode::GOTO);
	EXPECT_AT(fn->body, 1, std::int32_t(0));
	EXPECT_AT(fn->body, 5, XOpCode::NOP);
}

TEST(TestXFunctionBuilder, ForwardGoto) {
	XFunctionBuilder fb;
	auto label = fb.mklabel();
	fb.emitGoto(label);
	fb.emitNop();
	fb.emitNop();
	fb.place(label);
	fb.emitNop();

	auto fn = fb.finalize();

	EXPECT_AT(fn->body, 0, XOpCode::GOTO);
	EXPECT_AT(fn->body, 1, std::int32_t(2));
	EXPECT_AT(fn->body, 5, XOpCode::NOP);
	EXPECT_AT(fn->body, 6, XOpCode::NOP);
	EXPECT_AT(fn->body, 7, XOpCode::NOP);
}

TEST(TestXFunctionBuilder, ForwardGoto2) {
	XFunctionBuilder fb;
	auto label = fb.mklabel();
	fb.emitGoto(label);
	fb.emitNop();
	fb.emitNop();
	fb.emitNop();
	fb.place(label);
	fb.emitHalt();

	auto fn = fb.finalize();

	EXPECT_AT(fn->body, 0, XOpCode::GOTO);
	EXPECT_AT(fn->body, 1, std::int32_t(3));
	EXPECT_AT(fn->body, 5, XOpCode::NOP);
	EXPECT_AT(fn->body, 6, XOpCode::NOP);
	EXPECT_AT(fn->body, 7, XOpCode::NOP);
	EXPECT_AT(fn->body, 8, XOpCode::HALT);
}

TEST(TestCompiler, TestEmptyFunc) {
	Ab::XCompiler compiler;

	Cc::Compilation c;

	Val v = emitI32Add(c, emitI32(c, 42), emitI32(c, 2));
	
	IrValue value = Compiler::emitAdd(Compiler::emitI32Const(42), Compiler::emitI32Const(2));
	Compi
}

#endif ////////////

}  // namespace Ab
