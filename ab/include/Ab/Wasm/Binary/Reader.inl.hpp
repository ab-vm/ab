#ifndef AB_WASM_BINARY_READER_INL_HPP_
#define AB_WASM_BINARY_READER_INL_HPP_

#include <Ab/Wasm/Binary/Reader.hpp>

namespace Ab {
namespace Wasm {
namespace Binary {

inline Reader::Reader(Visitor& visitor, std::istream& in) : visitor_{visitor}, in_{in} {
}

inline auto Reader::operator()() -> void {
	module();
}

inline auto Reader::module() -> void {
	visitor_.moduleStart();
	header();
	sections();
	visitor_.moduleEnd();
}

inline auto Reader::header() -> void {
	visitor_.header();
	magic();
	version();
}

inline auto Reader::magic() -> void {
	auto magic = uint32();
	if (magic != ModuleHeader::MAGIC) {
		throw InvalidHeader{};
	}
}

inline auto Reader::version() -> void {
	auto version = uint32();
	if (version != ModuleHeader::VERSION) {
		throw InvalidHeader{};
	}
}

auto Reader::sections() -> void {
	auto count = uleb128();
	for (std::size_t i = 0; i < count; i++) {
		section();
	}
}

inline auto Reader::section() -> void {
	auto code = sectionCode();
	visitor_.sectionStart(code);

	switch (code) {
	case SectionCode::TYPE:
		typeSection();
		break;
	case SectionCode::IMPORT:
		importSection();
		break;
	case SectionCode::NAME:
	case SectionCode::FUNCTION:
	case SectionCode::TABLE:
	case SectionCode::MEMORY:
	case SectionCode::GLOBAL:
	case SectionCode::EXPORT:
	case SectionCode::START:
	case SectionCode::ELEMENT:
	case SectionCode::CODE:
	case SectionCode::DATA:
	default:
		throw InvalidSection();
		break;
	}
	visitor_.sectionEnd(code);
}

inline auto Reader::sectionCode() -> SectionCode {
	auto n = varuint7();  // varuint7
	return (SectionCode)n;
}

inline auto Reader::typeSection() -> void {
	auto count = leb128();
	visitor_.typeSection(count);
	for (auto i = 0; i < count; i++) {
		typeEntry();
	}
}

inline auto Reader::typeEntry() -> void{
	// todo
};

inline auto Reader::importSection() -> void {
	auto count = leb128();
	visitor_.importSection(count);
	for (auto i = 0; i < count; i++) {
		importEntry();
	}
}

inline auto Reader::importEntry() -> void {
	// String module, field;
	// read(module);
	// read(field);
	// visitor_.importEntry(module, field);
}

inline auto Reader::valueType() -> TypeCode {
	auto n = varint7();
	return (TypeCode)n;
}

inline auto Reader::blockType() -> TypeCode {
	auto n = varint7();
	return (TypeCode)n;
}

inline auto Reader::elementType() -> TypeCode {
	auto n = varint7();
	return (TypeCode)n;
}

inline auto Reader::functionType() -> void {
}

template <typename Integer, std::size_t bytes>
inline auto Reader::uint() -> Integer {
	if (in_.fail())
		throw BadNumber();

	Integer result = 0;
	auto buffer    = (char*)&result;
	in_.read((char*)&buffer, bytes);
	return result;
}

inline auto Reader::uint8() -> std::uint8_t {
	return uint<std::uint8_t>();
}

inline auto Reader::uint16() -> std::uint16_t {
	return uint<std::uint16_t>();
}

inline auto Reader::uint32() -> std::uint32_t {
	return uint<std::uint32_t>();
}

inline auto Reader::uint64() -> std::uint64_t {
	return uint<std::uint64_t>();
}

inline auto Reader::leb128() -> std::int64_t {
	const std::uint8_t FLAG = 0b1000'0000;
	const std::uint8_t MASK = 0b0111'1111;
	const std::uint8_t SIGN = 0b0100'0000;

	std::uint64_t result = 0;
	std::size_t shift    = 0;
	std::uint8_t byte    = 0;

	do {
		if (shift > 63)
			throw BadNumber{};
		byte = in_.get();
		result |= std::uint64_t(byte & MASK) << shift;
		shift += 7;

	} while (byte & FLAG);

	if (byte & SIGN) {
		result |= ~0 << shift;
	}

	return result;
}

inline auto Reader::varint7() -> std::int64_t {
	return leb128();
}

inline auto Reader::varint32() -> std::int64_t {
	return leb128();
}

inline auto Reader::varint64() -> std::int64_t {
	return leb128();
}

inline auto Reader::uleb128() -> std::uint64_t {
	constexpr std::uint8_t FLAG = 0b1000'0000;
	constexpr std::uint8_t MASK = 0b0111'1111;

	std::uint64_t result = 0;
	std::uint64_t byte   = 0;
	std::size_t shift    = 0;

	do {
		if (shift > 63)
			throw BadNumber{};
		const std::uint8_t byte = in_.get();
		result |= std::uint64_t(byte & MASK) << shift;
		shift += 7;
	} while (byte & FLAG);

	return result;
}

inline auto Reader::varuint1() -> std::uint64_t {
	return uleb128();
}

inline auto Reader::varuint7() -> std::uint64_t {
	return uleb128();
}

inline auto Reader::varuint32() -> std::uint64_t {
	return uleb128();
}

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_READER_INL_HPP_
