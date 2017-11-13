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
	auto m = magic();
	auto v = version();
	visitor_.header(m, v);
}

inline auto Reader::magic() -> std::uint32_t {
	auto magic = uint32();
	if (magic != ModuleHeader::MAGIC) {
		throw InvalidHeader{};
	}
	return magic;
}

inline auto Reader::version() -> std::uint32_t {
	auto version = uint32();
	if (version != ModuleHeader::VERSION) {
		throw InvalidHeader{};
	}
	return version;
}

auto Reader::sections() -> void {
	while (!in_.eof()) {
		auto code = sectionCode();
		if (in_.eof()) {
			break;
		}
		section(code);
	}
}

inline auto Reader::section(SectionCode code) -> void {
	Section section;
	section.code   = code;
	section.length = varuint32();

	if (section.code == SectionCode::NAME) {
		section.name = string();
	}

	visitor_.sectionStart(section);

	switch (section.code) {
	case SectionCode::TYPE:
		typeSection(section);
		break;
	case SectionCode::IMPORT:
		importSection(section);
		break;
	case SectionCode::NAME:
		customSection(section);
		break;
	case SectionCode::FUNCTION:
		functionSection(section);
		break;
	case SectionCode::TABLE:
		tableSection(section);
		break;
	case SectionCode::MEMORY:
		memorySection(section);
		break;
	case SectionCode::GLOBAL:
		globalSection(section);
		break;
	case SectionCode::EXPORT:
		exportSection(section);
		break;
	case SectionCode::START:
		startSection(section);
		break;
	case SectionCode::ELEMENT:
		elementSection(section);
		break;
	case SectionCode::CODE:
		codeSection(section);
		break;
	case SectionCode::DATA:
		dataSection(section);
		break;
	default:
		throw InvalidSection();
		break;
	}

	visitor_.sectionEnd(section);
}

inline auto Reader::sectionCode() -> SectionCode {
	auto n = varuint7();  // varuint7
	return (SectionCode)n;
}

/// The type section contains the types of all functions.
/// Every func
inline auto Reader::typeSection(const Section& section) -> void {
	auto count = varuint32();
	visitor_.typeSection(count);
	for (std::size_t i = 0; i < count; i++) {
		functionType();
	}
}

inline auto Reader::functionType() -> void {
	FunctionType result;
	result.hasReturnType = false;
	result.returnType    = TypeCode::EMPTY;

	auto type = typeCode();

	if (type != TypeCode::FUNC)
		throw ReaderError{"Expected function type, got something else."};

	auto paramCount = varuint32();

	result.paramTypes.resize(paramCount);

	for (std::size_t i = 0; i < paramCount; i++) {
		auto paramType       = valueType();
		result.paramTypes[i] = paramType;
	}

	auto hasReturnType = varuint1();

	if (hasReturnType) {
		auto returnType      = valueType();
		result.hasReturnType = true;
		result.returnType    = returnType;
	}

	visitor_.functionType(result);
};

inline auto Reader::importSection(const Section& section) -> void {
	auto count = varuint32();
	visitor_.importSection(count);
	for (std::size_t i = 0; i < count; i++) {
		importEntry();
	}
}

inline auto Reader::importEntry() -> void {
	ImportEntry entry;

	entry.module = string();
	entry.field  = string();
	entry.kind   = externalKind();

	switch (entry.kind) {
	case ExternalKindCode::FUNCTION:
		entry.type.function = varuint32();
		break;
	case ExternalKindCode::TABLE:
		tableType(entry.type.table);
		break;
	case ExternalKindCode::MEMORY:
		memoryType(entry.type.memory);
		break;
	case ExternalKindCode::GLOBAL:
		globalType(entry.type.global);
		break;
	default:
		throw ReaderError{"Unknown external kind code"};
		break;
	}

	visitor_.importEntry(entry);
}

inline auto Reader::externalKind() -> ExternalKindCode {
	return (ExternalKindCode)uint8();
}

inline auto Reader::functionSection(const Section& section) -> void {
	auto count = varuint32();
	for (std::size_t i = 0; i < count; i++) {
		auto type = varuint32();
		visitor_.functionEntry(i, type);
	}
}

inline auto Reader::customSection(const Section& section) -> void {
	// We don't do anything :)
}

inline auto Reader::string() -> std::string {
	std::string result;

	auto n   = varuint32();
	auto buf = new char[n + 1];
	in_.read(buf, n);
	buf[n] = '\0';

	result.reserve(n);
	result = buf;

	delete[] buf;
	return result;
}

/// Table Section
inline auto Reader::tableSection(const Section& section) -> void {
}

/// Memory Section
inline auto Reader::memorySection(const Section& section) -> void {
}

/// Global Section
inline auto Reader::globalSection(const Section& section) -> void {
	auto count = varuint32();
	for (std::size_t i = 0; i < count; i++) {
		globalEntry();
	}
}

inline auto Reader::globalEntry() -> void {
	GlobalType type;
	globalType(type);
	auto expr = initExpression();

	visitor_.globalEntry(type, expr);
};

inline auto Reader::initExpression() -> Expression {
	Expression expr = expression();
	switch (expr.op) {
	case OpCode::I32_CONST:
	case OpCode::I64_CONST:
	case OpCode::F32_CONST:
	case OpCode::F64_CONST:
	case OpCode::GET_GLOBAL:
		break;
	default:
		throw ReaderError{"Illegal epression in init_expr"};
		break;
	}

	Expression end = expression();
	if (end.op != OpCode::END) {
		throw ReaderError{"init_expr must be 1 expression and the end bytecode."};
	}

	return expr;
};

inline auto Reader::expression() -> Expression {
	Expression expr;
	expr.op = opCode();

	switch (expr.op) {
	case OpCode::I32_CONST:
		expr.immediate.int32 = varint32();
		break;
	case OpCode::I64_CONST:
		expr.immediate.int64 = varint64();
		break;
	case OpCode::F32_CONST:
		expr.immediate.float32 = (float)uint32();
		break;
	case OpCode::F64_CONST:
		expr.immediate.float64 = (double)uint64();
		break;
	case OpCode::GET_GLOBAL:
		expr.immediate.uint32 = varuint32();
		break;
	case OpCode::END:
		/// No operand
		break;
	default:
		throw ReaderError{std::string{"unhandled opcode: "} + toString(expr.op)};
		break;
	}
	return expr;
}

inline auto Reader::opCode() -> OpCode {
	return (OpCode)uint8();
}

/// Export Section
inline auto Reader::exportSection(const Section& section) -> void {
	std::size_t count = varuint32();
	for (std::size_t i = 0; i < count; i++) {
		exportEntry();
	}
}

inline auto Reader::exportEntry() -> void {
	ExportEntry entry;
	entry.field = string();
	entry.kind  = externalKind();
	entry.index = varuint32();
	visitor_.exportEntry(entry);
}

/// Start Section
inline auto Reader::startSection(const Section& section) -> void {
}

/// Element Section

inline auto Reader::elementSection(const Section& section) -> void {
	std::size_t count = varuint32();
	for (std::size_t i = 0; i < count; i++) {
		elementEntry();
	}
}

inline auto Reader::elementEntry() -> void {
	ElementEntry entry;
	entry.index        = varuint32();
	entry.offset       = initExpression();
	entry.elementCount = varuint32();
	visitor_.elementEntry(entry);
	for (std::size_t i = 0; i < entry.elementCount; i++) {
		auto index = varuint32();
		visitor_.element(entry, index);
	}
	visitor_.elementEntryEnd(entry);
}

/// Code Section
inline auto Reader::codeSection(const Section& section) -> void {
	std::size_t count = varuint32();
	for (std::size_t i = 0; i < count; i++) {
		functionBody(i);
	}
}

inline auto Reader::functionBody(std::size_t index) -> void {
	FunctionBody body;
	body.bodySize   = varuint32();
	auto localCount = varuint32();
	body.locals.resize(localCount);
	for (std::size_t i = 0; i < localCount; i++) {
		body.locals[i] = localEntry();
	}

	// TODO: Use body size.
	visitor_.functionBody(index, body);

	Expression expr;
	do {
		expr = expression();
		visitor_.functionBodyExpression(body, expr);
	} while (expr.op != OpCode::END);

	visitor_.functionBodyEnd(body);
}

inline auto Reader::localEntry() -> LocalEntry {
	LocalEntry entry;
	entry.count = varuint32();
	entry.type  = valueType();
	return entry;
}

/// Data Section

inline auto Reader::dataSection(const Section& section) -> void {
}

/// Common Values

inline auto Reader::resizableLimits(ResizableLimits& out) -> void {
	out.hasMax  = varuint1();
	out.initial = varuint32();
	if (out.hasMax) {
		out.max = varuint32();
	} else {
		out.max = 0;
	}
}

inline auto Reader::globalType(GlobalType& out) -> void {
	out.contentType = valueType();
	out.isMutable   = varuint1();
}

inline auto Reader::tableType(TableType& out) -> void {
	out.elementType = elementType();
	resizableLimits(out.limits);
}

inline auto Reader::memoryType(MemoryType& out) -> void {
	resizableLimits(out.limits);
}

inline auto Reader::valueType() -> TypeCode {
	return typeCode();
}

inline auto Reader::blockType() -> TypeCode {
	return typeCode();
}

inline auto Reader::elementType() -> TypeCode {
	return typeCode();
}

inline auto Reader::typeCode() -> TypeCode {
	// Note that while typecodes are actually leb128 encoded, we don't convert the constant to
	// it's proper value. We leave the typecodes encoded.
	auto n = uint8();
	return (TypeCode)n;
}

template <typename Integer, std::size_t bytes>
inline auto Reader::uint() -> Integer {
	if (in_.fail())
		throw BadNumber();

	Integer result = 0;
	auto buffer    = (char*)&result;
	in_.read(buffer, bytes);
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

	} while (byte & FLAG && (shift < 64) && !in_.eof());

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
	std::size_t shift    = 0;
	std::uint8_t byte    = 0;

	do {
		byte = in_.get();
		result |= std::uint64_t(byte & MASK) << shift;
		shift += 7;
	} while ((byte & FLAG) != 0 && shift < 64 && !in_.eof());

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
