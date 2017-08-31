#ifndef AB_SECTION_HPP_
#define AB_SECTION_HPP_

#include <cstdint>

namespace Ab {
namespace Wasm {

enum class SectionCode : std::uint8_t {
	NAME = 00,
	TYPE = 01,
	IMPORT = 02,
	FUNCTION = 03,
	TABLE = 04,
	MEMORY = 05,
	GLOBAL = 06,
	EXPORT = 07,
	START = 08,
	ELEMENT = 09,
	CODE = 10,
	DATA = 11
};

}  // namespace Wasm
}  // namespace Ab

#endif  // AB_SECTION_HPP_
