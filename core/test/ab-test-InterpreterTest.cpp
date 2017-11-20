#include <Ab/Config.hpp>
#include "Ab/Test/BasicTest.hpp"
#include <Ab/Instruction.hpp>
#include <Ab/Interpreter.hpp>
#include <gtest/gtest.h>

#include <Ab/XFunctionBuilder.hpp>

namespace Ab::Test {

#if 0 /////////////////////////////

// class TestInterpreter : public BasicTest {};

TEST(TestInterpreter, LoadEmptyModule) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitNop();
        fb.emitHalt();

        fn = fb.finalize();
    }

    Interpreter().interpretInt32(fn);

    delete[] fn;
}

TEST(TestInterpreter, BranchOverNop) {
    const XFunction* fn = nullptr;

    {
        XFunctionBuilder fb;
        auto lbl = fb.mklabel();
        fb.emitGoto(lbl);
        fb.emitNop();
        fb.place(lbl);
        fb.emitHalt();
        fn = fb.finalize();
    }

    Interpreter().interpretNil(fn);

    delete[] fn;
}

void my_primitive(Interpreter* interpreter) {
    fprintf(stderr, "my primitive was called\n");
};

TEST(TestInterpreter, CallPrimitive) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitCallPrimitive((uintptr_t)&my_primitive);
        fb.emitHalt();
        fn = fb.finalize();
    }

    Interpreter().interpretNil(fn);

    delete[] fn;
}

TEST(TestInterpreter, ReturnInt32) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitI32Const(0xcafe);
        fb.emitHalt();
        fn = fb.finalize();
    }
    EXPECT_EQ(Interpreter().interpretInt32(fn), 0xcafe);
    delete[] fn;
}

TEST(TestInterpreter, X32SelectTrue) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitI32Const(4);
        fb.emitI32Const(5);
        fb.emitI32Const(1);
        fb.emitX32Select();
        fb.emitHalt();
        fn = fb.finalize();
    }
    EXPECT_EQ(Interpreter().interpretInt32(fn), 4);
    delete[] fn;
}

TEST(TestInterpreter, X32SelectFalse) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitI32Const(4);
        fb.emitI32Const(5);
        fb.emitI32Const(0);
        fb.emitX32Select();
        fb.emitHalt();
        fn = fb.finalize();
    }
    EXPECT_EQ(Interpreter().interpretInt32(fn), 5);
    delete[] fn;
}

TEST(TestInterpreter, I32EqzTrue) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitI32Const(0);
        fb.emitI32Eqz();
        fb.emitHalt();
        fn = fb.finalize();
    }
    EXPECT_EQ(Interpreter().interpretInt32(fn), 1);
    delete[] fn;
}

TEST(TestInterpreter, I32EqzFalse) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitI32Const(1);
        fb.emitI32Eqz();
        fb.emitHalt();
        fn = fb.finalize();
    }
    EXPECT_EQ(Interpreter().interpretInt32(fn), 0);
    delete[] fn;
}

TEST(TestInterpreter, I32EqTrue) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitI32Const(42);
        fb.emitI32Const(42);
        fb.emitI32Eq();
        fb.emitHalt();
        fn = fb.finalize();
    }
    EXPECT_EQ(Interpreter().interpretInt32(fn), 1);
    delete[] fn;
}

TEST(TestInterpreter, I32EqFalse) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitI32Const(42);
        fb.emitI32Const(41);
        fb.emitI32Eq();
        fb.emitHalt();
        fn = fb.finalize();
    }
    EXPECT_EQ(Interpreter().interpretInt32(fn), 0);
    delete[] fn;
}

TEST(TestInterpreter, I32Add) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitI32Const(4);
        fb.emitI32Const(6);
        fb.emitI32Add();
        fb.emitHalt();
        fn = fb.finalize();
    }
    EXPECT_EQ(Interpreter().interpretInt32(fn), 10);
    delete[] fn;
}

TEST(TestInterpreter, I32Sub) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitI32Const(4);
        fb.emitI32Const(6);
        fb.emitI32Sub();
        fb.emitHalt();
        fn = fb.finalize();
    }
    EXPECT_EQ(Interpreter().interpretInt32(fn), -2);
    delete[] fn;
}

TEST(TestInterpreter, I32Shiftl) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
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
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitI32Const(4);
        fb.emitI32Const(32);
        fb.emitI32Shl();
        fn = fb.finalize();
    }
    EXPECT_EQ(Interpreter().interpretInt32(fn), 4);
    delete[] fn;
}

TEST(TestInterpreter, I32Shiftl_WrapShiftBitsTo1) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        fb.emitI32Const(4);
        fb.emitI32Const(33);
        fb.emitI32Shl();
        fn = fb.finalize();
    }
    EXPECT_EQ(Interpreter().interpretInt32(fn), 8);
    delete[] fn;
}

TEST(TestInterpreter, GotoIfTrue) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        auto label = fb.mklabel();
        fb.emitI32Const(1);
        fb.emitGotoIf(label);
        fb.emitI32Const(2);
        fb.emitHalt();
        fb.place(label);
        fb.emitI32Const(3);
        fb.emitHalt();
        fn = fb.finalize();
    }
    EXPECT_EQ(Interpreter().interpretInt32(fn), 3);
    delete[] fn;
}

TEST(TestInterpreter, ReturnArgument) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        auto param = fb.declX32Param();
        fb.emitX32GetLocal(param);
        fb.emitHalt();
        fn = fb.finalize();
    }
    Interpreter interpreter;
    interpreter.pushParam<std::int32_t>(42);
    EXPECT_EQ(interpreter.interpretInt32(fn), 42);
    delete[] fn;
}

TEST(TestInterpreter, AddToArgument) {
    const XFunction* fn = nullptr;
    {
        XFunctionBuilder fb;
        auto param = fb.declX32Param();
        fb.emitX32GetLocal(param);
        fb.emitI32Const(14);
        fb.emitI32Add();
        fb.emitHalt();
        fn = fb.finalize();
    }
    Interpreter interpreter;
    interpreter.pushParam<std::int32_t>(42);
    EXPECT_EQ(interpreter.interpretInt32(fn), 56);
    delete[] fn;
}

#endif //////////////////////////////

}  // namespace Ab::Test
