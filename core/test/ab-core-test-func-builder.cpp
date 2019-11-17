#if 0

#include <Ab/XDisasm.hpp>
#include <Ab/FuncBuilder.hpp>
#include <Ab/Opcode.hpp>
#include <gtest/gtest.h>

namespace Ab::Test {

template <typename T>
T& interpret(Byte* buffer, std::size_t offset) {
	return *reinterpret_cast<T*>(buffer + offset);
}

template <typename T>
testing::AssertionResult equal_at(Byte* buffer, std::size_t offset, const T& expected) {
	const auto& value = interpret<const T>(buffer, offset);

	testing::Message msg;
	msg << (void*)buffer << "[" << offset << "]" << "=" << value;

	if (value == expected) {
		return testing::AssertionSuccess() << msg;
	}
	return testing::AssertionFailure()  << msg;
}

#define EXPECT_AT(buffer, offset, value) EXPECT_TRUE(equal_at(buffer, offset, value))

void print_program(FILE* file, const Byte* program, std::size_t nbytes) {
	for (std::size_t i = 0; i < nbytes; ++i) {
		fprintf(file, "%p[%lu]=%u\n", program, i, uint(program[i]));
	}
}

TEST(TestFuncBuilder, EmptyFunction) {
	FuncBuilder fb;
	fb.emit_nop();
	fb.emit_nop();
	fb.emit_nop();
	fb.emit_nop();
	fb.emit_halt();

	auto fn = fb.finalize();

	EXPECT_EQ(fn->header.nparams, 0);
	EXPECT_EQ(fn->header.nlocals, 0);

	EXPECT_AT(fn->body, 0, Opcode::NOP);
	EXPECT_AT(fn->body, 1, Opcode::NOP);
	EXPECT_AT(fn->body, 2, Opcode::NOP);
	EXPECT_AT(fn->body, 3, Opcode::NOP);
	EXPECT_AT(fn->body, 4, Opcode::HALT);
}

TEST(TestFuncBuilder, GotoSamePc) {
	FuncBuilder fb;
	auto label = fb.place();
	fb.emit_goto(label);

	auto fn = fb.finalize();

	EXPECT_AT(fn->body, 0, Opcode::GOTO);
	EXPECT_AT(fn->body, 1, std::int32_t(-5));

	delete fn;
}

TEST(TestFuncBuilder, GotoForward0) {
	FuncBuilder fb;
	auto label = fb.make_label();
	fb.emit_goto(label);
	fb.place(label);
	fb.emit_nop();

	auto fn = fb.finalize();

	EXPECT_AT(fn->body, 0, Opcode::GOTO);
	EXPECT_AT(fn->body, 1, std::int32_t(0));
	EXPECT_AT(fn->body, 5, Opcode::NOP);

	delete fn;
}

TEST(TestFuncBuilder, ForwardGoto) {
	FuncBuilder fb;
	auto label = fb.make_label();
	fb.emit_goto(label);
	fb.emit_nop();
	fb.emit_nop();
	fb.place(label);
	fb.emit_nop();

	auto fn = fb.finalize();

	EXPECT_AT(fn->body, 0, Opcode::GOTO);
	EXPECT_AT(fn->body, 1, std::int32_t(2));
	EXPECT_AT(fn->body, 5, Opcode::NOP);
	EXPECT_AT(fn->body, 6, Opcode::NOP);
	EXPECT_AT(fn->body, 7, Opcode::NOP);

	delete fn;
}

TEST(TestFuncBuilder, ForwardGoto2) {
	FuncBuilder fb;
	auto label = fb.make_label();
	fb.emit_goto(label);
	fb.emit_nop();
	fb.emit_nop();
	fb.emit_nop();
	fb.place(label);
	fb.emit_halt();

	auto fn = fb.finalize();

	EXPECT_AT(fn->body, 0, Opcode::GOTO);
	EXPECT_AT(fn->body, 1, std::int32_t(3));
	EXPECT_AT(fn->body, 5, Opcode::NOP);
	EXPECT_AT(fn->body, 6, Opcode::NOP);
	EXPECT_AT(fn->body, 7, Opcode::NOP);
	EXPECT_AT(fn->body, 8, Opcode::HALT);

	delete fn;
}

TEST(TestFuncBuilder, BackwardsGoto) {
	FuncBuilder fb;

	auto label = fb.place();
	fb.emit_nop();
	fb.emit_nop();
	fb.emit_nop();
	fb.emit_goto(label);

	auto fn = fb.finalize();

	EXPECT_AT(fn->body, 0, Opcode::NOP);
	EXPECT_AT(fn->body, 1, Opcode::NOP);
	EXPECT_AT(fn->body, 2, Opcode::NOP);
	EXPECT_AT(fn->body, 3, Opcode::GOTO);
	EXPECT_AT(fn->body, 4, std::int32_t(-8));

	delete fn;
}

}  // namespace Ab::Test

#endif // 0
