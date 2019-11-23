#ifndef AB_MODULECONSTANTS_HPP_
#define AB_MODULECONSTANTS_HPP_

#include <cstdint>

namespace Ab {

/// The magic constant at the beginning of every ab module.
/// In LE, it's the string literal "abab".
///
constexpr std::uint32_t MODULE_MAGIC = 0x62616261;

constexpr std::uint32_t MODULE_VERSION = 1;

enum class SectionCode : std::uint8_t {
	NAME    = 0x0,
	TYPE    = 0x1,
	IMPORT  = 0x2,
	FUNC    = 0x3,
	TABLE   = 0x4,
	MEMORY  = 0x5,
	GLOBAL  = 0x6,
	EXPORT  = 0x7,
	START   = 0x8,
	ELEMENT = 0x9,
	CODE    = 0xa,
	DATA    = 0xb
};

enum class ValType : std::uint8_t {
	I32     = 0x7f,  // -0x01
	I64     = 0x7e,  // -0x02
	F32     = 0x7d,  // -0x03
	F64     = 0x7c,  // -0x04
	ANYFUNC = 0x70,  // -0x10
	FUNC    = 0x60,  // -0x20
	EMPTY   = 0x40   // -0x40
};

}  // namespace Ab

#endif  // AB_MODULECONSTANTS_HPP_
