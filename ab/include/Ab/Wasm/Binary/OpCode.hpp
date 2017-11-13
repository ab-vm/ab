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

inline auto toString(OpCode code) -> const char* {
	switch (code) {
	case OpCode::UNREACHABLE:
		return "unreachable";
		break;
	case OpCode::NOP:
		return "nop";
		break;
	case OpCode::BLOCK:
		return "block";
		break;
	case OpCode::LOOP:
		return "loop";
		break;
	case OpCode::IF:
		return "if";
		break;
	case OpCode::ELSE:
		return "else";
		break;
	case OpCode::END:
		return "end";
		break;
	case OpCode::BR:
		return "br";
		break;
	case OpCode::BR_IF:
		return "br_if";
		break;
	case OpCode::BR_TABLE:
		return "br_table";
		break;
	case OpCode::RETURN:
		return "return";
		break;
	case OpCode::CALL:
		return "call";
		break;
	case OpCode::CALL_INDIRECT:
		return "call_indirect";
		break;
	case OpCode::DROP:
		return "drop";
		break;
	case OpCode::SELECT:
		return "select";
		break;
	case OpCode::GET_LOCAL:
		return "get_local";
		break;
	case OpCode::SET_LOCAL:
		return "set_local";
		break;
	case OpCode::TEE_LOCAL:
		return "tee_local";
		break;
	case OpCode::GET_GLOBAL:
		return "get_global";
		break;
	case OpCode::SET_GLOBAL:
		return "set_global";
		break;
	case OpCode::I32_LOAD:
		return "i32.load";
		break;
	case OpCode::I64_LOAD:
		return "i64.load";
		break;
	case OpCode::F32_LOAD:
		return "f32.load";
		break;
	case OpCode::F64_LOAD:
		return "f64.load";
		break;
	case OpCode::I32_LOAD8_S:
		return "i32.load8_s";
		break;
	case OpCode::I32_LOAD8_U:
		return "i32.load8_u";
		break;
	case OpCode::I32_LOAD16_S:
		return "i32.load16_s";
		break;
	case OpCode::I32_LOAD16_U:
		return "i32.load16_u";
		break;
	case OpCode::I64_LOAD8_S:
		return "i64.load8_s";
		break;
	case OpCode::I64_LOAD8_U:
		return "i64.load8_u";
		break;
	case OpCode::I64_LOAD16_S:
		return "i64.load16_s";
		break;
	case OpCode::I64_LOAD16_U:
		return "i64.load16_u";
		break;
	case OpCode::I64_LOAD32_S:
		return "i64.load32_s";
		break;
	case OpCode::I64_LOAD32_U:
		return "i64.load32_u";
		break;
	case OpCode::I32_STORE:
		return "i32.store";
		break;
	case OpCode::I64_STORE:
		return "i64.store";
		break;
	case OpCode::F32_STORE:
		return "f32.store";
		break;
	case OpCode::F64_STORE:
		return "f64.store";
		break;
	case OpCode::I32_STORE8:
		return "i32.store8";
		break;
	case OpCode::I32_STORE16:
		return "i32.store16";
		break;
	case OpCode::I64_STORE8:
		return "i64.store8";
		break;
	case OpCode::I64_STORE16:
		return "i64.store16";
		break;
	case OpCode::I64_STORE32:
		return "i64.store32";
		break;
	case OpCode::CURRENT_MEMORY:
		return "current_memory";
		break;
	case OpCode::GROW_MEMORY:
		return "grow_memory";
		break;
	case OpCode::I32_CONST:
		return "i32.const";
		break;
	case OpCode::I64_CONST:
		return "i64.const";
		break;
	case OpCode::F32_CONST:
		return "f32.const";
		break;
	case OpCode::F64_CONST:
		return "f64.const";
		break;
	default:
		return "UNKNOWN_OPCODE";
	}
}

inline auto operator<<(std::ostream& out, OpCode code) -> std::ostream& {
	return out << toString(code);
}

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_BYTECODE_HPP_
