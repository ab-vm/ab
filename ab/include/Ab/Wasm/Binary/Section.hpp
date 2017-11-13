#ifndef AB_WASM_BINARY_SECTION_HPP_
#define AB_WASM_BINARY_SECTION_HPP_

#include <Ab/Config.hpp>
#include <Pith/OStream.hpp>

#include <cstddef>
#include <cstdint>
#include <ostream>
#include <string>

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

inline auto toRaw(SectionCode code) -> RawSectionCode {
	return RawSectionCode(code);
}

inline auto toString(SectionCode code) -> const char* {
	switch (code) {
	case SectionCode::NAME:
		return "NAME";
	case SectionCode::TYPE:
		return "TYPE";
	case SectionCode::IMPORT:
		return "IMPORT";
	case SectionCode::FUNCTION:
		return "FUNCTION";
	case SectionCode::TABLE:
		return "TABLE";
	case SectionCode::MEMORY:
		return "MEMORY";
	case SectionCode::GLOBAL:
		return "GLOBAL";
	case SectionCode::EXPORT:
		return "EXPORT";
	case SectionCode::START:
		return "START";
	case SectionCode::ELEMENT:
		return "ELEMENT";
	case SectionCode::CODE:
		return "CODE";
	case SectionCode::DATA:
		return "DATA";
	default:
		return "UNKNOWN_SECTION_CODE";
	}
}

inline auto operator<<(std::ostream& out, SectionCode code) -> std::ostream& {
	return out << toString(code);
}

struct Section {
	SectionCode code;
	std::uint32_t length;
	std::string name;
};

inline auto operator<<(Pith::SexprPrinter& out, const Section& section) -> Pith::SexprPrinter& {
	out << Pith::sexprStart << "section";
	out << Pith::sexprStart << "code" << section.code << Pith::sexprEnd;

	if (section.code == SectionCode::NAME) {
		out << Pith::sexprStart << "name" << section.name << Pith::sexprEnd;
	}

	out << Pith::sexprStart << "size" << section.length << Pith::sexprEnd;

	out << Pith::sexprEnd;

	return out;
}

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_BINARY_SECTION_HPP_
