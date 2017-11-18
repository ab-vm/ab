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
	F64_CONST = 0x44,

	/// Comparison Operators
	/// No Immediates

	I32_EQZ  = 0x45,
	I32_EQ   = 0x46,
	I32_NE   = 0x47,
	I32_LT_S = 0x48,
	I32_LT_U = 0x49,
	I32_GT_S = 0x4a,
	I32_GT_U = 0x4b,
	I32_LE_S = 0x4c,
	I32_LE_U = 0x4d,
	I32_GE_S = 0x4e,
	I32_GE_U = 0x4f,

	I64_EQZ  = 0x50,
	I64_EQ   = 0x51,
	I64_NE   = 0x52,
	I64_LT_S = 0x53,
	I64_LT_U = 0x54,
	I64_GT_S = 0x55,
	I64_GT_U = 0x56,
	I64_LE_S = 0x57,
	I64_LE_U = 0x58,
	I64_GE_S = 0x59,
	I64_GE_U = 0x5a,

	F32_EQ = 0x5b,
	F32_NE = 0x5c,
	F32_LT = 0x5d,
	F32_GT = 0x5e,
	F32_LE = 0x5f,
	F32_GE = 0x60,

	F64_EQ = 0x61,
	F64_NE = 0x62,
	F64_LT = 0x63,
	F64_GT = 0x64,
	F64_LE = 0x65,
	F64_GE = 0x66,

	/// Numeric Operators
	/// No immediates.

	I32_CLZ    = 0x67,
	I32_CTZ    = 0x68,
	I32_POPCNT = 0x69,
	I32_ADD    = 0x6a,
	I32_SUB    = 0x6b,
	I32_MUL    = 0x6c,
	I32_DIV_S  = 0x6d,
	I32_DIV_U  = 0x6e,
	I32_REM_S  = 0x6f,
	I32_REM_U  = 0x70,
	I32_AND    = 0x71,
	I32_OR     = 0x72,
	I32_XOR    = 0x73,
	I32_SHL    = 0x74,
	I32_SHR_S  = 0x75,
	I32_SHR_U  = 0x76,
	I32_ROTL   = 0x77,
	I32_ROTR   = 0x78,

	I64_CLZ    = 0x79,
	I64_CTZ    = 0x7a,
	I64_POPCNT = 0x7b,
	I64_ADD    = 0x7c,
	I64_SUB    = 0x7d,
	I64_MUL    = 0x7e,
	I64_DIV_S  = 0x7f,
	I64_DIV_U  = 0x80,
	I64_REM_S  = 0x81,
	I64_REM_U  = 0x82,
	I64_AND    = 0x83,
	I64_OR     = 0x84,
	I64_XOR    = 0x85,
	I64_SHL    = 0x86,
	I64_SHR_S  = 0x87,
	I64_SHR_U  = 0x88,
	I64_ROTL   = 0x89,
	I64_ROTR   = 0x8a,

	F32_ABS      = 0x8b,
	F32_NEG      = 0x8c,
	F32_CEIL     = 0x8d,
	F32_FLOOR    = 0x8e,
	F32_TRUNC    = 0x8f,
	F32_NEAREST  = 0x90,
	F32_SQRT     = 0x91,
	F32_ADD      = 0x92,
	F32_SUB      = 0x93,
	F32_MUL      = 0x94,
	F32_DIV      = 0x95,
	F32_MIN      = 0x96,
	F32_MAX      = 0x97,
	F32_COPYSIGN = 0x98,

	F64_ABS      = 0x99,
	F64_NEG      = 0x9a,
	F64_CEIL     = 0x9b,
	F64_FLOOR    = 0x9c,
	F64_TRUNC    = 0x9d,
	F64_NEAREST  = 0x9e,
	F64_SQRT     = 0x9f,
	F64_ADD      = 0xa0,
	F64_SUB      = 0xa1,
	F64_MUL      = 0xa2,
	F64_DIV      = 0xa3,
	F64_MIN      = 0xa4,
	F64_MAX      = 0xa5,
	F64_COPYSIGN = 0xa6,

	/// TODO: Conversions
	I32_WRAP_I64      = 0xa7,
	I32_TRUNC_S_F32   = 0xa8,
	I32_TRUNC_U_F32   = 0xa9,
	I32_TRUNC_S_F64   = 0xaa,
	I32_TRUNC_U_F64   = 0xab,
	I64_EXTEND_S_I32  = 0xac,
	I64_EXTEND_U_I32  = 0xad,
	I64_TRUNC_S_F32   = 0xae,
	I64_TRUNC_U_F32   = 0xaf,
	I64_TRUNC_S_F64   = 0xb0,
	I64_TRUNC_U_F64   = 0xb1,
	F32_CONVERT_S_I32 = 0xb2,
	F32_CONVERT_U_I32 = 0xb3,
	F32_CONVERT_S_I64 = 0xb4,
	F32_CONVERT_U_I64 = 0xb5,
	F32_DEMOTE_F64    = 0xb6,
	F64_CONVERT_S_I32 = 0xb7,
	F64_CONVERT_U_I32 = 0xb8,
	F64_CONVERT_S_I64 = 0xb9,
	F64_CONVERT_U_I64 = 0xba,
	F64_PROMOTE_F32   = 0xbb,

	/// TODO: Reinterpretations
	I32_REINTERPRET_F32 = 0xbc,
	I64_REINTERPRET_F64 = 0xbd,
	F32_REINTERPRET_I32 = 0xbe,
	F64_REINTERPRET_I64 = 0xbf
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
	case OpCode::I32_EQZ:
		return Function<OpCode::I32_EQZ>()(std::forward<Args>(args)...);
	case OpCode::I32_EQ:
		return Function<OpCode::I32_EQ>()(std::forward<Args>(args)...);
	case OpCode::I32_NE:
		return Function<OpCode::I32_NE>()(std::forward<Args>(args)...);
	case OpCode::I32_LT_S:
		return Function<OpCode::I32_LT_S>()(std::forward<Args>(args)...);
	case OpCode::I32_LT_U:
		return Function<OpCode::I32_LT_U>()(std::forward<Args>(args)...);
	case OpCode::I32_GT_S:
		return Function<OpCode::I32_GT_S>()(std::forward<Args>(args)...);
	case OpCode::I32_GT_U:
		return Function<OpCode::I32_GT_U>()(std::forward<Args>(args)...);
	case OpCode::I32_LE_S:
		return Function<OpCode::I32_LE_S>()(std::forward<Args>(args)...);
	case OpCode::I32_LE_U:
		return Function<OpCode::I32_LE_U>()(std::forward<Args>(args)...);
	case OpCode::I32_GE_S:
		return Function<OpCode::I32_GE_S>()(std::forward<Args>(args)...);
	case OpCode::I32_GE_U:
		return Function<OpCode::I32_GE_U>()(std::forward<Args>(args)...);
	case OpCode::I64_EQZ:
		return Function<OpCode::I64_EQZ>()(std::forward<Args>(args)...);
	case OpCode::I64_EQ:
		return Function<OpCode::I64_EQ>()(std::forward<Args>(args)...);
	case OpCode::I64_NE:
		return Function<OpCode::I64_NE>()(std::forward<Args>(args)...);
	case OpCode::I64_LT_S:
		return Function<OpCode::I64_LT_S>()(std::forward<Args>(args)...);
	case OpCode::I64_LT_U:
		return Function<OpCode::I64_LT_U>()(std::forward<Args>(args)...);
	case OpCode::I64_GT_S:
		return Function<OpCode::I64_GT_S>()(std::forward<Args>(args)...);
	case OpCode::I64_GT_U:
		return Function<OpCode::I64_GT_U>()(std::forward<Args>(args)...);
	case OpCode::I64_LE_S:
		return Function<OpCode::I64_LE_S>()(std::forward<Args>(args)...);
	case OpCode::I64_LE_U:
		return Function<OpCode::I64_LE_U>()(std::forward<Args>(args)...);
	case OpCode::I64_GE_S:
		return Function<OpCode::I64_GE_S>()(std::forward<Args>(args)...);
	case OpCode::I64_GE_U:
		return Function<OpCode::I64_GE_U>()(std::forward<Args>(args)...);
	case OpCode::F32_EQ:
		return Function<OpCode::F32_EQ>()(std::forward<Args>(args)...);
	case OpCode::F32_NE:
		return Function<OpCode::F32_NE>()(std::forward<Args>(args)...);
	case OpCode::F32_LT:
		return Function<OpCode::F32_LT>()(std::forward<Args>(args)...);
	case OpCode::F32_GT:
		return Function<OpCode::F32_GT>()(std::forward<Args>(args)...);
	case OpCode::F32_LE:
		return Function<OpCode::F32_LE>()(std::forward<Args>(args)...);
	case OpCode::F32_GE:
		return Function<OpCode::F32_GE>()(std::forward<Args>(args)...);
	case OpCode::F64_EQ:
		return Function<OpCode::F64_EQ>()(std::forward<Args>(args)...);
	case OpCode::F64_NE:
		return Function<OpCode::F64_NE>()(std::forward<Args>(args)...);
	case OpCode::F64_LT:
		return Function<OpCode::F64_LT>()(std::forward<Args>(args)...);
	case OpCode::F64_GT:
		return Function<OpCode::F64_GT>()(std::forward<Args>(args)...);
	case OpCode::F64_LE:
		return Function<OpCode::F64_LE>()(std::forward<Args>(args)...);
	case OpCode::F64_GE:
		return Function<OpCode::F64_GE>()(std::forward<Args>(args)...);
	case OpCode::I32_CLZ:
		return Function<OpCode::I32_CLZ>()(std::forward<Args>(args)...);
	case OpCode::I32_CTZ:
		return Function<OpCode::I32_CTZ>()(std::forward<Args>(args)...);
	case OpCode::I32_POPCNT:
		return Function<OpCode::I32_POPCNT>()(std::forward<Args>(args)...);
	case OpCode::I32_ADD:
		return Function<OpCode::I32_ADD>()(std::forward<Args>(args)...);
	case OpCode::I32_SUB:
		return Function<OpCode::I32_SUB>()(std::forward<Args>(args)...);
	case OpCode::I32_MUL:
		return Function<OpCode::I32_MUL>()(std::forward<Args>(args)...);
	case OpCode::I32_DIV_S:
		return Function<OpCode::I32_DIV_S>()(std::forward<Args>(args)...);
	case OpCode::I32_DIV_U:
		return Function<OpCode::I32_DIV_U>()(std::forward<Args>(args)...);
	case OpCode::I32_REM_S:
		return Function<OpCode::I32_REM_S>()(std::forward<Args>(args)...);
	case OpCode::I32_REM_U:
		return Function<OpCode::I32_REM_U>()(std::forward<Args>(args)...);
	case OpCode::I32_AND:
		return Function<OpCode::I32_AND>()(std::forward<Args>(args)...);
	case OpCode::I32_OR:
		return Function<OpCode::I32_OR>()(std::forward<Args>(args)...);
	case OpCode::I32_XOR:
		return Function<OpCode::I32_XOR>()(std::forward<Args>(args)...);
	case OpCode::I32_SHL:
		return Function<OpCode::I32_SHL>()(std::forward<Args>(args)...);
	case OpCode::I32_SHR_S:
		return Function<OpCode::I32_SHR_S>()(std::forward<Args>(args)...);
	case OpCode::I32_SHR_U:
		return Function<OpCode::I32_SHR_U>()(std::forward<Args>(args)...);
	case OpCode::I32_ROTL:
		return Function<OpCode::I32_ROTL>()(std::forward<Args>(args)...);
	case OpCode::I32_ROTR:
		return Function<OpCode::I32_ROTR>()(std::forward<Args>(args)...);
	case OpCode::I64_CLZ:
		return Function<OpCode::I64_CLZ>()(std::forward<Args>(args)...);
	case OpCode::I64_CTZ:
		return Function<OpCode::I64_CTZ>()(std::forward<Args>(args)...);
	case OpCode::I64_POPCNT:
		return Function<OpCode::I64_POPCNT>()(std::forward<Args>(args)...);
	case OpCode::I64_ADD:
		return Function<OpCode::I64_ADD>()(std::forward<Args>(args)...);
	case OpCode::I64_SUB:
		return Function<OpCode::I64_SUB>()(std::forward<Args>(args)...);
	case OpCode::I64_MUL:
		return Function<OpCode::I64_MUL>()(std::forward<Args>(args)...);
	case OpCode::I64_DIV_S:
		return Function<OpCode::I64_DIV_S>()(std::forward<Args>(args)...);
	case OpCode::I64_DIV_U:
		return Function<OpCode::I64_DIV_U>()(std::forward<Args>(args)...);
	case OpCode::I64_REM_S:
		return Function<OpCode::I64_REM_S>()(std::forward<Args>(args)...);
	case OpCode::I64_REM_U:
		return Function<OpCode::I64_REM_U>()(std::forward<Args>(args)...);
	case OpCode::I64_AND:
		return Function<OpCode::I64_AND>()(std::forward<Args>(args)...);
	case OpCode::I64_OR:
		return Function<OpCode::I64_OR>()(std::forward<Args>(args)...);
	case OpCode::I64_XOR:
		return Function<OpCode::I64_XOR>()(std::forward<Args>(args)...);
	case OpCode::I64_SHL:
		return Function<OpCode::I64_SHL>()(std::forward<Args>(args)...);
	case OpCode::I64_SHR_S:
		return Function<OpCode::I64_SHR_S>()(std::forward<Args>(args)...);
	case OpCode::I64_SHR_U:
		return Function<OpCode::I64_SHR_U>()(std::forward<Args>(args)...);
	case OpCode::I64_ROTL:
		return Function<OpCode::I64_ROTL>()(std::forward<Args>(args)...);
	case OpCode::I64_ROTR:
		return Function<OpCode::I64_ROTR>()(std::forward<Args>(args)...);
	case OpCode::F32_ABS:
		return Function<OpCode::F32_ABS>()(std::forward<Args>(args)...);
	case OpCode::F32_NEG:
		return Function<OpCode::F32_NEG>()(std::forward<Args>(args)...);
	case OpCode::F32_CEIL:
		return Function<OpCode::F32_CEIL>()(std::forward<Args>(args)...);
	case OpCode::F32_FLOOR:
		return Function<OpCode::F32_FLOOR>()(std::forward<Args>(args)...);
	case OpCode::F32_TRUNC:
		return Function<OpCode::F32_TRUNC>()(std::forward<Args>(args)...);
	case OpCode::F32_NEAREST:
		return Function<OpCode::F32_NEAREST>()(std::forward<Args>(args)...);
	case OpCode::F32_SQRT:
		return Function<OpCode::F32_SQRT>()(std::forward<Args>(args)...);
	case OpCode::F32_ADD:
		return Function<OpCode::F32_ADD>()(std::forward<Args>(args)...);
	case OpCode::F32_SUB:
		return Function<OpCode::F32_SUB>()(std::forward<Args>(args)...);
	case OpCode::F32_MUL:
		return Function<OpCode::F32_MUL>()(std::forward<Args>(args)...);
	case OpCode::F32_DIV:
		return Function<OpCode::F32_DIV>()(std::forward<Args>(args)...);
	case OpCode::F32_MIN:
		return Function<OpCode::F32_MIN>()(std::forward<Args>(args)...);
	case OpCode::F32_MAX:
		return Function<OpCode::F32_MAX>()(std::forward<Args>(args)...);
	case OpCode::F32_COPYSIGN:
		return Function<OpCode::F32_COPYSIGN>()(std::forward<Args>(args)...);
	case OpCode::F64_ABS:
		return Function<OpCode::F64_ABS>()(std::forward<Args>(args)...);
	case OpCode::F64_NEG:
		return Function<OpCode::F64_NEG>()(std::forward<Args>(args)...);
	case OpCode::F64_CEIL:
		return Function<OpCode::F64_CEIL>()(std::forward<Args>(args)...);
	case OpCode::F64_FLOOR:
		return Function<OpCode::F64_FLOOR>()(std::forward<Args>(args)...);
	case OpCode::F64_TRUNC:
		return Function<OpCode::F64_TRUNC>()(std::forward<Args>(args)...);
	case OpCode::F64_NEAREST:
		return Function<OpCode::F64_NEAREST>()(std::forward<Args>(args)...);
	case OpCode::F64_SQRT:
		return Function<OpCode::F64_SQRT>()(std::forward<Args>(args)...);
	case OpCode::F64_ADD:
		return Function<OpCode::F64_ADD>()(std::forward<Args>(args)...);
	case OpCode::F64_SUB:
		return Function<OpCode::F64_SUB>()(std::forward<Args>(args)...);
	case OpCode::F64_MUL:
		return Function<OpCode::F64_MUL>()(std::forward<Args>(args)...);
	case OpCode::F64_DIV:
		return Function<OpCode::F64_DIV>()(std::forward<Args>(args)...);
	case OpCode::F64_MIN:
		return Function<OpCode::F64_MIN>()(std::forward<Args>(args)...);
	case OpCode::F64_MAX:
		return Function<OpCode::F64_MAX>()(std::forward<Args>(args)...);
	case OpCode::F64_COPYSIGN:
		return Function<OpCode::F64_COPYSIGN>()(std::forward<Args>(args)...);
	case OpCode::I32_WRAP_I64:
		return Function<OpCode::I32_WRAP_I64>()(std::forward<Args>(args)...);
	case OpCode::I32_TRUNC_S_F32:
		return Function<OpCode::I32_TRUNC_S_F32>()(std::forward<Args>(args)...);
	case OpCode::I32_TRUNC_U_F32:
		return Function<OpCode::I32_TRUNC_U_F32>()(std::forward<Args>(args)...);
	case OpCode::I32_TRUNC_S_F64:
		return Function<OpCode::I32_TRUNC_S_F64>()(std::forward<Args>(args)...);
	case OpCode::I32_TRUNC_U_F64:
		return Function<OpCode::I32_TRUNC_U_F64>()(std::forward<Args>(args)...);
	case OpCode::I64_EXTEND_S_I32:
		return Function<OpCode::I64_EXTEND_S_I32>()(std::forward<Args>(args)...);
	case OpCode::I64_EXTEND_U_I32:
		return Function<OpCode::I64_EXTEND_U_I32>()(std::forward<Args>(args)...);
	case OpCode::I64_TRUNC_S_F32:
		return Function<OpCode::I64_TRUNC_S_F32>()(std::forward<Args>(args)...);
	case OpCode::I64_TRUNC_U_F32:
		return Function<OpCode::I64_TRUNC_U_F32>()(std::forward<Args>(args)...);
	case OpCode::I64_TRUNC_S_F64:
		return Function<OpCode::I64_TRUNC_S_F64>()(std::forward<Args>(args)...);
	case OpCode::I64_TRUNC_U_F64:
		return Function<OpCode::I64_TRUNC_U_F64>()(std::forward<Args>(args)...);
	case OpCode::F32_CONVERT_S_I32:
		return Function<OpCode::F32_CONVERT_S_I32>()(std::forward<Args>(args)...);
	case OpCode::F32_CONVERT_U_I32:
		return Function<OpCode::F32_CONVERT_U_I32>()(std::forward<Args>(args)...);
	case OpCode::F32_CONVERT_S_I64:
		return Function<OpCode::F32_CONVERT_S_I64>()(std::forward<Args>(args)...);
	case OpCode::F32_CONVERT_U_I64:
		return Function<OpCode::F32_CONVERT_U_I64>()(std::forward<Args>(args)...);
	case OpCode::F32_DEMOTE_F64:
		return Function<OpCode::F32_DEMOTE_F64>()(std::forward<Args>(args)...);
	case OpCode::F64_CONVERT_S_I32:
		return Function<OpCode::F64_CONVERT_S_I32>()(std::forward<Args>(args)...);
	case OpCode::F64_CONVERT_U_I32:
		return Function<OpCode::F64_CONVERT_U_I32>()(std::forward<Args>(args)...);
	case OpCode::F64_CONVERT_S_I64:
		return Function<OpCode::F64_CONVERT_S_I64>()(std::forward<Args>(args)...);
	case OpCode::F64_CONVERT_U_I64:
		return Function<OpCode::F64_CONVERT_U_I64>()(std::forward<Args>(args)...);
	case OpCode::F64_PROMOTE_F32:
		return Function<OpCode::F64_PROMOTE_F32>()(std::forward<Args>(args)...);
	case OpCode::I32_REINTERPRET_F32:
		return Function<OpCode::I32_REINTERPRET_F32>()(std::forward<Args>(args)...);
	case OpCode::I64_REINTERPRET_F64:
		return Function<OpCode::I64_REINTERPRET_F64>()(std::forward<Args>(args)...);
	case OpCode::F32_REINTERPRET_I32:
		return Function<OpCode::F32_REINTERPRET_I32>()(std::forward<Args>(args)...);
	case OpCode::F64_REINTERPRET_I64:
		return Function<OpCode::F64_REINTERPRET_I64>()(std::forward<Args>(args)...);
	default:
		throw std::runtime_error{"unknown opcode"};
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

template <>
constexpr const char* OP_NAME<OpCode::I32_EQZ> = "i32.eqz";

template <>
constexpr const char* OP_NAME<OpCode::I32_EQ> = "i32.eq";

template <>
constexpr const char* OP_NAME<OpCode::I32_NE> = "i32.ne";

template <>
constexpr const char* OP_NAME<OpCode::I32_LT_S> = "i32.lt_s";

template <>
constexpr const char* OP_NAME<OpCode::I32_LT_U> = "i32.lt_u";

template <>
constexpr const char* OP_NAME<OpCode::I32_GT_S> = "i32.gt_s";

template <>
constexpr const char* OP_NAME<OpCode::I32_GT_U> = "i32.gt_u";

template <>
constexpr const char* OP_NAME<OpCode::I32_LE_S> = "i32.le_s";

template <>
constexpr const char* OP_NAME<OpCode::I32_LE_U> = "i32.le_u";

template <>
constexpr const char* OP_NAME<OpCode::I32_GE_S> = "i32.ge_s";

template <>
constexpr const char* OP_NAME<OpCode::I32_GE_U> = "i32.ge_u";

template <>
constexpr const char* OP_NAME<OpCode::I64_EQZ> = "i64.eqz";

template <>
constexpr const char* OP_NAME<OpCode::I64_EQ> = "i64.eq";

template <>
constexpr const char* OP_NAME<OpCode::I64_NE> = "i64.ne";

template <>
constexpr const char* OP_NAME<OpCode::I64_LT_S> = "i64.lt_s";

template <>
constexpr const char* OP_NAME<OpCode::I64_LT_U> = "i64.lt_u";

template <>
constexpr const char* OP_NAME<OpCode::I64_GT_S> = "i64.gt_s";

template <>
constexpr const char* OP_NAME<OpCode::I64_GT_U> = "i64.gt_u";

template <>
constexpr const char* OP_NAME<OpCode::I64_LE_S> = "i64.le_s";

template <>
constexpr const char* OP_NAME<OpCode::I64_LE_U> = "i64.le_u";

template <>
constexpr const char* OP_NAME<OpCode::I64_GE_S> = "i64.ge_s";

template <>
constexpr const char* OP_NAME<OpCode::I64_GE_U> = "i64.ge_u";

template <>
constexpr const char* OP_NAME<OpCode::F32_EQ> = "f32.eq";

template <>
constexpr const char* OP_NAME<OpCode::F32_NE> = "f32.ne";

template <>
constexpr const char* OP_NAME<OpCode::F32_LT> = "f32.lt";

template <>
constexpr const char* OP_NAME<OpCode::F32_GT> = "f32.gt";

template <>
constexpr const char* OP_NAME<OpCode::F32_LE> = "f32.le";

template <>
constexpr const char* OP_NAME<OpCode::F32_GE> = "f32.ge";

template <>
constexpr const char* OP_NAME<OpCode::F64_EQ> = "f64.eq";

template <>
constexpr const char* OP_NAME<OpCode::F64_NE> = "f64.ne";

template <>
constexpr const char* OP_NAME<OpCode::F64_LT> = "f64.lt";

template <>
constexpr const char* OP_NAME<OpCode::F64_GT> = "f64.gt";

template <>
constexpr const char* OP_NAME<OpCode::F64_LE> = "f64.le";

template <>
constexpr const char* OP_NAME<OpCode::F64_GE> = "f64.ge";

template <>
constexpr const char* OP_NAME<OpCode::I32_CLZ> = "i32.clz";

template <>
constexpr const char* OP_NAME<OpCode::I32_CTZ> = "i32.ctz";

template <>
constexpr const char* OP_NAME<OpCode::I32_POPCNT> = "i32.popcnt";

template <>
constexpr const char* OP_NAME<OpCode::I32_ADD> = "i32.add";

template <>
constexpr const char* OP_NAME<OpCode::I32_SUB> = "i32.sub";

template <>
constexpr const char* OP_NAME<OpCode::I32_MUL> = "i32.mul";

template <>
constexpr const char* OP_NAME<OpCode::I32_DIV_S> = "i32.div_s";

template <>
constexpr const char* OP_NAME<OpCode::I32_DIV_U> = "i32.div_u";

template <>
constexpr const char* OP_NAME<OpCode::I32_REM_S> = "i32.rem_s";

template <>
constexpr const char* OP_NAME<OpCode::I32_REM_U> = "i32.rem_u";

template <>
constexpr const char* OP_NAME<OpCode::I32_AND> = "i32.and";

template <>
constexpr const char* OP_NAME<OpCode::I32_OR> = "i32.or";

template <>
constexpr const char* OP_NAME<OpCode::I32_XOR> = "i32.xor";

template <>
constexpr const char* OP_NAME<OpCode::I32_SHL> = "i32.shl";

template <>
constexpr const char* OP_NAME<OpCode::I32_SHR_S> = "i32.shr_s";

template <>
constexpr const char* OP_NAME<OpCode::I32_SHR_U> = "i32.shr_u";

template <>
constexpr const char* OP_NAME<OpCode::I32_ROTL> = "i32.rotl";

template <>
constexpr const char* OP_NAME<OpCode::I32_ROTR> = "i32.rotr";

template <>
constexpr const char* OP_NAME<OpCode::I64_CLZ> = "i64.clz";

template <>
constexpr const char* OP_NAME<OpCode::I64_CTZ> = "i64.ctz";

template <>
constexpr const char* OP_NAME<OpCode::I64_POPCNT> = "i64.popcnt";

template <>
constexpr const char* OP_NAME<OpCode::I64_ADD> = "i64.add";

template <>
constexpr const char* OP_NAME<OpCode::I64_SUB> = "i64.sub";

template <>
constexpr const char* OP_NAME<OpCode::I64_MUL> = "i64.mul";

template <>
constexpr const char* OP_NAME<OpCode::I64_DIV_S> = "i64.div_s";

template <>
constexpr const char* OP_NAME<OpCode::I64_DIV_U> = "i64.div_u";

template <>
constexpr const char* OP_NAME<OpCode::I64_REM_S> = "i64.rem_s";

template <>
constexpr const char* OP_NAME<OpCode::I64_REM_U> = "i64.rem_u";

template <>
constexpr const char* OP_NAME<OpCode::I64_AND> = "i64.and";

template <>
constexpr const char* OP_NAME<OpCode::I64_OR> = "i64.or";

template <>
constexpr const char* OP_NAME<OpCode::I64_XOR> = "i64.xor";

template <>
constexpr const char* OP_NAME<OpCode::I64_SHL> = "i64.shl";

template <>
constexpr const char* OP_NAME<OpCode::I64_SHR_S> = "i64.shr_s";

template <>
constexpr const char* OP_NAME<OpCode::I64_SHR_U> = "i64.shr_u";

template <>
constexpr const char* OP_NAME<OpCode::I64_ROTL> = "i64.rotl";

template <>
constexpr const char* OP_NAME<OpCode::I64_ROTR> = "i64.rotr";

template <>
constexpr const char* OP_NAME<OpCode::F32_ABS> = "f32.abs";

template <>
constexpr const char* OP_NAME<OpCode::F32_NEG> = "f32.neg";

template <>
constexpr const char* OP_NAME<OpCode::F32_CEIL> = "f32.ceil";

template <>
constexpr const char* OP_NAME<OpCode::F32_FLOOR> = "f32.floor";

template <>
constexpr const char* OP_NAME<OpCode::F32_TRUNC> = "f32.trunc";

template <>
constexpr const char* OP_NAME<OpCode::F32_NEAREST> = "f32.nearest";

template <>
constexpr const char* OP_NAME<OpCode::F32_SQRT> = "f32.sqrt";

template <>
constexpr const char* OP_NAME<OpCode::F32_ADD> = "f32.add";

template <>
constexpr const char* OP_NAME<OpCode::F32_SUB> = "f32.sub";

template <>
constexpr const char* OP_NAME<OpCode::F32_MUL> = "f32.mul";

template <>
constexpr const char* OP_NAME<OpCode::F32_DIV> = "f32.div";

template <>
constexpr const char* OP_NAME<OpCode::F32_MIN> = "f32.min";

template <>
constexpr const char* OP_NAME<OpCode::F32_MAX> = "f32.max";

template <>
constexpr const char* OP_NAME<OpCode::F32_COPYSIGN> = "f32.copysign";

template <>
constexpr const char* OP_NAME<OpCode::F64_ABS> = "f64.abs";

template <>
constexpr const char* OP_NAME<OpCode::F64_NEG> = "f64.neg";

template <>
constexpr const char* OP_NAME<OpCode::F64_CEIL> = "f64.ceil";

template <>
constexpr const char* OP_NAME<OpCode::F64_FLOOR> = "f64.floor";

template <>
constexpr const char* OP_NAME<OpCode::F64_TRUNC> = "f64.trunc";

template <>
constexpr const char* OP_NAME<OpCode::F64_NEAREST> = "f64.nearest";

template <>
constexpr const char* OP_NAME<OpCode::F64_SQRT> = "f64.sqrt";

template <>
constexpr const char* OP_NAME<OpCode::F64_ADD> = "f64.add";

template <>
constexpr const char* OP_NAME<OpCode::F64_SUB> = "f64.sub";

template <>
constexpr const char* OP_NAME<OpCode::F64_MUL> = "f64.mul";

template <>
constexpr const char* OP_NAME<OpCode::F64_DIV> = "f64.div";

template <>
constexpr const char* OP_NAME<OpCode::F64_MIN> = "f64.min";

template <>
constexpr const char* OP_NAME<OpCode::F64_MAX> = "f64.max";

template <>
constexpr const char* OP_NAME<OpCode::F64_COPYSIGN> = "f64.copysign";

template <>
constexpr const char* OP_NAME<OpCode::I32_WRAP_I64> = "i32.wrap/i64";

template <>
constexpr const char* OP_NAME<OpCode::I32_TRUNC_S_F32> = "i32.trunc_s/f32";

template <>
constexpr const char* OP_NAME<OpCode::I32_TRUNC_U_F32> = "i32.trunc_u/f32";

template <>
constexpr const char* OP_NAME<OpCode::I32_TRUNC_S_F64> = "i32.trunc_s/f64";

template <>
constexpr const char* OP_NAME<OpCode::I32_TRUNC_U_F64> = "i32.trunc_u/f64";

template <>
constexpr const char* OP_NAME<OpCode::I64_EXTEND_S_I32> = "i64.extend_s/i32";

template <>
constexpr const char* OP_NAME<OpCode::I64_EXTEND_U_I32> = "i64.extend_u/i32";

template <>
constexpr const char* OP_NAME<OpCode::I64_TRUNC_S_F32> = "i64.trunc_s/f32";

template <>
constexpr const char* OP_NAME<OpCode::I64_TRUNC_U_F32> = "i64.trunc_u/f32";

template <>
constexpr const char* OP_NAME<OpCode::I64_TRUNC_S_F64> = "i64.trunc_s/f64";

template <>
constexpr const char* OP_NAME<OpCode::I64_TRUNC_U_F64> = "i64.trunc_u/f64";

template <>
constexpr const char* OP_NAME<OpCode::F32_CONVERT_S_I32> = "f32.convert_s/i32";

template <>
constexpr const char* OP_NAME<OpCode::F32_CONVERT_U_I32> = "f32.convert_u/i32";

template <>
constexpr const char* OP_NAME<OpCode::F32_CONVERT_S_I64> = "f32.convert_s/i64";

template <>
constexpr const char* OP_NAME<OpCode::F32_CONVERT_U_I64> = "f32.convert_u/i64";

template <>
constexpr const char* OP_NAME<OpCode::F32_DEMOTE_F64> = "f32.demote/f64";

template <>
constexpr const char* OP_NAME<OpCode::F64_CONVERT_S_I32> = "f64.convert_s/i32";

template <>
constexpr const char* OP_NAME<OpCode::F64_CONVERT_U_I32> = "f64.convert_u/i32";

template <>
constexpr const char* OP_NAME<OpCode::F64_CONVERT_S_I64> = "f64.convert_s/i64";

template <>
constexpr const char* OP_NAME<OpCode::F64_CONVERT_U_I64> = "f64.convert_u/i64";

template <>
constexpr const char* OP_NAME<OpCode::F64_PROMOTE_F32> = "f64.promote/f32";

template <>
constexpr const char* OP_NAME<OpCode::I32_REINTERPRET_F32> = "i32.reinterpret/f32";

template <>
constexpr const char* OP_NAME<OpCode::I64_REINTERPRET_F64> = "i64.reinterpret/f64";

template <>
constexpr const char* OP_NAME<OpCode::F32_REINTERPRET_I32> = "f32.reinterpret/i32";

template <>
constexpr const char* OP_NAME<OpCode::F64_REINTERPRET_I64> = "f64.reinterpret/i64";

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
