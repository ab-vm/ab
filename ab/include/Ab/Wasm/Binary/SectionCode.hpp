#ifndef AB_WASM_BINARY_SECTION_HPP_
#define AB_WASM_BINARY_SECTION_HPP_

#include <cstdint>
#include <cstddef>

namespace Ab {
namespace Wasm {
namespace Binary {

using RawSectionCode = std::uint8_t;

enum class SectionCode : RawSectionCode {
	NAME     = 0x0,
	TYPE     = 0x1,
	IMPORT   = 0x2,
	FUNCTION = 0x3,
	TABLE    = 0x4,
	MEMORY   = 0x5,
	GLOBAL   = 0x6,
	EXPORT   = 0x7,
	START    = 0x8,
	ELEMENT  = 0x9,
	CODE     = 0xa,
	DATA     = 0xb
};

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_BINARY_SECTION_HPP_
