#ifndef AB_WASM_BYTECODE_HPP_
#define AB_WASM_BYTECODE_HPP_

#include <cstddef>
#include <cstdint>
#include <ostream>

namespace Ab {
namespace Wasm {
namespace Binary {

using RawOpCode = std::uint8_t;

/// Binary encoding of wasm operators.
/// http://webassembly.org/docs/binary-encoding/
/// http://webassembly.org/docs/semantics/
enum class OpCode : RawOpCode {

	/// Standard Codes

	/// Control flow operators
	/// http://webassembly.org/docs/semantics/#control-flow-structures

	/// Trap immediately.
	UNREACHABLE = 0x00,

	/// No operation
	NOP = 0x01,

	/// Begin a sequence of expressions, yielding 0 or 1 values.
	/// Immediates:
	///   sig: block_type
	BLOCK = 0x02,

	/// Begin a block which can also form control flow loops.
	LOOP = 0x03,

	/// Begin if expression.
	IF = 0x04,

	/// Begin else expression of if.
	ELSE = 0x05,

	/// End a block, loop, or if.
	END = 0x0b,

	/// Break that targets an outer nested block.
	/// Immediates:
	///   relative_depth: varuint32
	BR = 0x0c,

	/// Conditional break that targets an outer nested block.
	/// Immediates:
	///   relative_depth: varuint32
	BR_IF = 0x0d,

	/// Branch table control flow construct.
	/// Immediates:
	///   target_count:   varuint32
	///   target_table:   varuint32*
	///   default_target: varuint32
	BR_TABLE = 0x0e,

	/// Return 0 or 1 value from this function.
	RETURN = 0x0f,

	/// Call operators

	/// Call a function by it's index
	/// Immediates:
	///   function_index: varuint32
	CALL = 0x10,

	/// Call a function indirect with an expected signature
	/// Immediates:
	///   type_index: varuint32
	///   reserved:   varuint1
	/// This operator takes a list of function arguments
	/// `reserved` is for future use, and must be zero.
	CALL_INDIRECT = 0x11,

	/// Parametric operators

	/// Ignore value.
	DROP = 0x1a,

	/// Select one of two values based on condition.
	SELECT = 0x1b,

	/// Variable Access

	/// Read a local variable or parameter.
	/// Immediates:
	///   local_index: varuint32
	GET_LOCAL = 0x20,

	/// Write a local variable or parameter.
	/// Immediates:
	///   local_index: varuint32
	SET_LOCAL = 0x21,

	/// Write a local variable or parameter, then return the same value.
	/// Immediates:
	///   local_index: varuint32
	TEE_LOCAL = 0x22,

	/// Read a global variable.
	/// Immediates:
	///   global_index: varuint32
	GET_GLOBAL = 0x23,

	/// Write a global variable.
	/// Immediates:
	///   global_index: varuint32
	SET_GLOBAL = 0x24,

	/// Memory Operators

	/// Load from memory.
	/// Immediates:
	///   flags:  varuint32
	///   offset: varuint32
	/// The alignment of the address is store in the low bits of the flags as log2(n).
	I32_LOAD     = 0x28,
	I64_LOAD     = 0x29,
	F32_LOAD     = 0x2a,
	F64_LOAD     = 0x2b,
	I32_LOAD8_S  = 0x2c,
	I32_LOAD8_U  = 0x2d,
	I32_LOAD16_S = 0x2e,
	I32_LOAD16_U = 0x2f,
	I64_LOAD8_S  = 0x30,
	I64_LOAD8_U  = 0x31,
	I64_LOAD16_S = 0x32,
	I64_LOAD16_U = 0x33,
	I64_LOAD32_S = 0x34,
	I64_LOAD32_U = 0x35,

	/// Store to Memory
	/// Immediates:
	///   flags:  varuint32
	///   offset: varuint32
	I32_STORE   = 0x36,
	I64_STORE   = 0x37,
	F32_STORE   = 0x38,
	F64_STORE   = 0x39,
	I32_STORE8  = 0x3a,
	I32_STORE16 = 0x3b,
	I64_STORE8  = 0x3c,
	I64_STORE16 = 0x3d,
	I64_STORE32 = 0x3e,

	/// Query the size of memory.
	/// Immediates:
	///   reserved: varuint1
	CURRENT_MEMORY = 0x3f,

	/// Grow the size of memory.
	/// Immediates:
	///   reserved: varuint1
	GROW_MEMORY = 0x40,

	/// Constants

	/// A constant i32.
	/// Immediates:
	///   value: varint32
	I32_CONST = 0x41,

	/// A constant i64.
	/// Immediates:
	///   value: varint64
	I64_CONST = 0x42,

	/// A constant f32.
	/// Immediates:
	///   value: uint32
	F32_CONST = 0x43,

	/// A constant f64.
	/// Immediates:
	///   value: uint64
	F64_CONST = 0x44

	/// TODO: Comparison operators

	/// TODO: Numeric operators

	/// TODO: Conversions
};

template <template <OpCode> class Function, typename... Args>
inline auto opDispatch(OpCode op, Args&&... args)
	-> decltype(std::declval<Function<OpCode::UNREACHABLE>>()(std::forward<Args>(args)...)) {
	switch (op) {
	case OpCode::UNREACHABLE:
		return Function<OpCode::UNREACHABLE>()(std::forward<Args>(args)...);
	case OpCode::NOP:
		return Function<OpCode::NOP>()(std::forward<Args>(args)...);
	case OpCode::BLOCK:
		return Function<OpCode::BLOCK>()(std::forward<Args>(args)...);
	case OpCode::LOOP:
		return Function<OpCode::LOOP>()(std::forward<Args>(args)...);
	case OpCode::IF:
		return Function<OpCode::IF>()(std::forward<Args>(args)...);
	case OpCode::ELSE:
		return Function<OpCode::ELSE>()(std::forward<Args>(args)...);
	case OpCode::END:
		return Function<OpCode::END>()(std::forward<Args>(args)...);
	case OpCode::BR:
		return Function<OpCode::BR>()(std::forward<Args>(args)...);
	case OpCode::BR_IF:
		return Function<OpCode::BR_IF>()(std::forward<Args>(args)...);
	case OpCode::BR_TABLE:
		return Function<OpCode::BR_TABLE>()(std::forward<Args>(args)...);
	case OpCode::RETURN:
		return Function<OpCode::RETURN>()(std::forward<Args>(args)...);
	case OpCode::CALL:
		return Function<OpCode::CALL>()(std::forward<Args>(args)...);
	case OpCode::CALL_INDIRECT:
		return Function<OpCode::CALL_INDIRECT>()(std::forward<Args>(args)...);
	case OpCode::DROP:
		return Function<OpCode::DROP>()(std::forward<Args>(args)...);
	case OpCode::SELECT:
		return Function<OpCode::SELECT>()(std::forward<Args>(args)...);
	case OpCode::GET_LOCAL:
		return Function<OpCode::GET_LOCAL>()(std::forward<Args>(args)...);
	case OpCode::SET_LOCAL:
		return Function<OpCode::SET_LOCAL>()(std::forward<Args>(args)...);
	case OpCode::TEE_LOCAL:
		return Function<OpCode::TEE_LOCAL>()(std::forward<Args>(args)...);
	case OpCode::GET_GLOBAL:
		return Function<OpCode::GET_GLOBAL>()(std::forward<Args>(args)...);
	case OpCode::SET_GLOBAL:
		return Function<OpCode::SET_GLOBAL>()(std::forward<Args>(args)...);
	case OpCode::I32_LOAD:
		return Function<OpCode::I32_LOAD>()(std::forward<Args>(args)...);
	case OpCode::I64_LOAD:
		return Function<OpCode::I64_LOAD>()(std::forward<Args>(args)...);
	case OpCode::F32_LOAD:
		return Function<OpCode::F32_LOAD>()(std::forward<Args>(args)...);
	case OpCode::F64_LOAD:
		return Function<OpCode::F64_LOAD>()(std::forward<Args>(args)...);
	case OpCode::I32_LOAD8_S:
		return Function<OpCode::I32_LOAD8_S>()(std::forward<Args>(args)...);
	case OpCode::I32_LOAD8_U:
		return Function<OpCode::I32_LOAD8_U>()(std::forward<Args>(args)...);
	case OpCode::I32_LOAD16_S:
		return Function<OpCode::I32_LOAD16_S>()(std::forward<Args>(args)...);
	case OpCode::I32_LOAD16_U:
		return Function<OpCode::I32_LOAD16_U>()(std::forward<Args>(args)...);
	case OpCode::I64_LOAD8_S:
		return Function<OpCode::I64_LOAD8_S>()(std::forward<Args>(args)...);
	case OpCode::I64_LOAD8_U:
		return Function<OpCode::I64_LOAD8_U>()(std::forward<Args>(args)...);
	case OpCode::I64_LOAD16_S:
		return Function<OpCode::I64_LOAD16_S>()(std::forward<Args>(args)...);
	case OpCode::I64_LOAD16_U:
		return Function<OpCode::I64_LOAD16_U>()(std::forward<Args>(args)...);
	case OpCode::I64_LOAD32_S:
		return Function<OpCode::I64_LOAD32_S>()(std::forward<Args>(args)...);
	case OpCode::I64_LOAD32_U:
		return Function<OpCode::I64_LOAD32_U>()(std::forward<Args>(args)...);
	case OpCode::I32_STORE:
		return Function<OpCode::I32_STORE>()(std::forward<Args>(args)...);
	case OpCode::I64_STORE:
		return Function<OpCode::I64_STORE>()(std::forward<Args>(args)...);
	case OpCode::F32_STORE:
		return Function<OpCode::F32_STORE>()(std::forward<Args>(args)...);
	case OpCode::F64_STORE:
		return Function<OpCode::F64_STORE>()(std::forward<Args>(args)...);
	case OpCode::I32_STORE8:
		return Function<OpCode::I32_STORE8>()(std::forward<Args>(args)...);
	case OpCode::I32_STORE16:
		return Function<OpCode::I32_STORE16>()(std::forward<Args>(args)...);
	case OpCode::I64_STORE8:
		return Function<OpCode::I64_STORE8>()(std::forward<Args>(args)...);
	case OpCode::I64_STORE16:
		return Function<OpCode::I64_STORE16>()(std::forward<Args>(args)...);
	case OpCode::I64_STORE32:
		return Function<OpCode::I64_STORE32>()(std::forward<Args>(args)...);
	case OpCode::CURRENT_MEMORY:
		return Function<OpCode::CURRENT_MEMORY>()(std::forward<Args>(args)...);
	case OpCode::GROW_MEMORY:
		return Function<OpCode::GROW_MEMORY>()(std::forward<Args>(args)...);
	case OpCode::I32_CONST:
		return Function<OpCode::I32_CONST>()(std::forward<Args>(args)...);
	case OpCode::I64_CONST:
		return Function<OpCode::I64_CONST>()(std::forward<Args>(args)...);
	case OpCode::F32_CONST:
		return Function<OpCode::F32_CONST>()(std::forward<Args>(args)...);
	case OpCode::F64_CONST:
		return Function<OpCode::F64_CONST>()(std::forward<Args>(args)...);
	}
}

template <OpCode op>
constexpr const char* OP_NAME = "UNKNOWN_OP";

template <>
constexpr const char* OP_NAME<OpCode::UNREACHABLE> = "unreachable";

template <>
constexpr const char* OP_NAME<OpCode::NOP> = "nop";

template <>
constexpr const char* OP_NAME<OpCode::BLOCK> = "block";

template <>
constexpr const char* OP_NAME<OpCode::LOOP> = "loop";

template <>
constexpr const char* OP_NAME<OpCode::IF> = "if";

template <>
constexpr const char* OP_NAME<OpCode::ELSE> = "else";

template <>
constexpr const char* OP_NAME<OpCode::END> = "end";

template <>
constexpr const char* OP_NAME<OpCode::BR> = "br";

template <>
constexpr const char* OP_NAME<OpCode::BR_IF> = "br_if";

template <>
constexpr const char* OP_NAME<OpCode::BR_TABLE> = "br_table";

template <>
constexpr const char* OP_NAME<OpCode::RETURN> = "return";

template <>
constexpr const char* OP_NAME<OpCode::CALL> = "call";

template <>
constexpr const char* OP_NAME<OpCode::CALL_INDIRECT> = "call_indirect";

template <>
constexpr const char* OP_NAME<OpCode::DROP> = "drop";

template <>
constexpr const char* OP_NAME<OpCode::SELECT> = "select";

template <>
constexpr const char* OP_NAME<OpCode::GET_LOCAL> = "get_local";

template <>
constexpr const char* OP_NAME<OpCode::SET_LOCAL> = "set_local";

template <>
constexpr const char* OP_NAME<OpCode::TEE_LOCAL> = "tee_local";

template <>
constexpr const char* OP_NAME<OpCode::GET_GLOBAL> = "get_global";

template <>
constexpr const char* OP_NAME<OpCode::SET_GLOBAL> = "set_global";

template <>
constexpr const char* OP_NAME<OpCode::I32_LOAD> = "i32.load";

template <>
constexpr const char* OP_NAME<OpCode::I64_LOAD> = "i64.load";

template <>
constexpr const char* OP_NAME<OpCode::F32_LOAD> = "f32.load";

template <>
constexpr const char* OP_NAME<OpCode::F64_LOAD> = "f64.load";

template <>
constexpr const char* OP_NAME<OpCode::I32_LOAD8_S> = "i32.load8_s";

template <>
constexpr const char* OP_NAME<OpCode::I32_LOAD8_U> = "i32.load8_u";

template <>
constexpr const char* OP_NAME<OpCode::I32_LOAD16_S> = "i32.load16_s";

template <>
constexpr const char* OP_NAME<OpCode::I32_LOAD16_U> = "i32.load16_u";

template <>
constexpr const char* OP_NAME<OpCode::I64_LOAD8_S> = "i64.load8_s";

template <>
constexpr const char* OP_NAME<OpCode::I64_LOAD8_U> = "i64.load8_u";

template <>
constexpr const char* OP_NAME<OpCode::I64_LOAD16_S> = "i64.load16_s";

template <>
constexpr const char* OP_NAME<OpCode::I64_LOAD16_U> = "i64.load16_u";

template <>
constexpr const char* OP_NAME<OpCode::I64_LOAD32_S> = "i64.load32_s";

template <>
constexpr const char* OP_NAME<OpCode::I64_LOAD32_U> = "i64.load32_u";

template <>
constexpr const char* OP_NAME<OpCode::I32_STORE> = "i32.store";

template <>
constexpr const char* OP_NAME<OpCode::I64_STORE> = "i64.store";

template <>
constexpr const char* OP_NAME<OpCode::F32_STORE> = "f32.store";

template <>
constexpr const char* OP_NAME<OpCode::F64_STORE> = "f64.store";

template <>
constexpr const char* OP_NAME<OpCode::I32_STORE8> = "i32.store8";

template <>
constexpr const char* OP_NAME<OpCode::I32_STORE16> = "i32.store16";

template <>
constexpr const char* OP_NAME<OpCode::I64_STORE8> = "i64.store8";

template <>
constexpr const char* OP_NAME<OpCode::I64_STORE16> = "i64.store16";

template <>
constexpr const char* OP_NAME<OpCode::I64_STORE32> = "i64.store32";

template <>
constexpr const char* OP_NAME<OpCode::CURRENT_MEMORY> = "current_memory";

template <>
constexpr const char* OP_NAME<OpCode::GROW_MEMORY> = "grow_memory";

template <>
constexpr const char* OP_NAME<OpCode::I32_CONST> = "i32.const";

template <>
constexpr const char* OP_NAME<OpCode::I64_CONST> = "i64.const";

template <>
constexpr const char* OP_NAME<OpCode::F32_CONST> = "f32.const";

template <>
constexpr const char* OP_NAME<OpCode::F64_CONST> = "f64.const";

/// Instantiates FunctionTemplate as
///   FunctionTemplate<OpCode>
template <OpCode op>
struct NameOfOp {
	auto operator()() -> const char* {
		return OP_NAME<op>;
	}
};

inline auto toString(OpCode op) -> const char* {
	return opDispatch<NameOfOp>(op);
}

inline auto operator<<(std::ostream& out, OpCode code) -> std::ostream& {
	return out << toString(code);
}

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_BYTECODE_HPP_