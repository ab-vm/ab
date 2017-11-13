#ifndef AB_WASM_BINARY_TYPECODE_HPP_
#define AB_WASM_BINARY_TYPECODE_HPP_

#include <cstdint>

namespace Ab {
namespace Wasm {
namespace Binary {

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

inline auto toString(TypeCode code) -> const char* {
	switch (code) {
	case TypeCode::I32:
		return "i32";
	case TypeCode::I64:
		return "i64";
	case TypeCode::F32:
		return "f32";
	case TypeCode::F64:
		return "f64";
	case TypeCode::ANYFUNC:
		return "anyfunc";
	case TypeCode::FUNC:
		return "func";
	case TypeCode::EMPTY:
		return "empty";
	default:
		return "UNKNOWN_TYPECODE";
	}
};

inline auto operator<<(std::ostream& out, TypeCode code) -> std::ostream& {
	return out << toString(code);
}

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_BINARY_TYPECODE_HPP_
