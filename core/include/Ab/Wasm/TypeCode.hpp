#ifndef AB_WASM_BINARY_TYPECODE_HPP_
#define AB_WASM_BINARY_TYPECODE_HPP_

#include <cstdint>

namespace Ab::Wasm {

using RawTypeCode = std::uint8_t;

enum class TypeCode : RawTypeCode {
	I32     = 0x7f,  // -0x01
	I64     = 0x7e,  // -0x02
	F32     = 0x7d,  // -0x03
	F64     = 0x7c,  // -0x04
	ANYFUNC = 0x70,  // -0x10
	FUNC    = 0x60,  // -0x20
	EMPTY   = 0x40   // -0x40
};

}  // namespace Ab::Wasm

#endif  // AB_WASM_BINARY_TYPECODE_HPP_
