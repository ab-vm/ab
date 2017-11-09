#ifndef AB_INSTRUCTION_HPP_
#define AB_INSTRUCTION_HPP_

#include <Ab/Config.hpp>
#include <Ab/Wasm/Binary/OpCode.hpp>
#include <cstdint>

namespace Ab {

/// Internal AB interpreter instructions.
/// Note: not true WASM instructions.
/// @see-also: Ab::Wasm::Bytecode
enum class Instruction : std::uint8_t {
	/// Trap immediately.
	UNREACHABLE = 0x00,  // Wasm::Bytecode::UNREACHABLE

	/// No operation
	NOP = 0x01  // Wasm::Bytecode::NOP

#if 0
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
	I32_LOAD = 0x28,
	I64_LOAD = 0x29,
	F32_LOAD = 0x2a,
	F64_LOAD = 0x2b,
	I32_LOAD8_S = 0x2c,
	I32_LOAD8_U = 0x2d,
	I32_LOAD16_S = 0x2e,
	I32_LOAD16_U = 0x2f,
	I64_LOAD8_S = 0x30,
	I64_LOAD8_U = 0x31,
	I64_LOAD16_S = 0x32,
	I64_LOAD16_U = 0x33,
	I64_LOAD32_S = 0x34,
	I64_LOAD32_U = 0x35,

	/// Store to Memory
	/// Immediates:
	///   flags:  varuint32
	///   offset: varuint32
	I32_STORE = 0x36,
	I64_STORE = 0x37,
	F32_STORE = 0x38,
	F64_STORE = 0x39,
	I32_STORE8 = 0x3a,
	I32_STORE16 = 0x3b,
	I64_STORE8 = 0x3c,
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
#endif  // 0

};

}  // namespace Ab

#endif  // AB_INSTRUCTION_HPP_
