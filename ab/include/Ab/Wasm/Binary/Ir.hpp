#ifndef AB_WASM_BINARY_IR_HPP_
#define AB_WASM_BINARY_IR_HPP_

#include <Ab/Config.hpp>
#include <Ab/Wasm/Binary/OpCode.hpp>
#include <Ab/Wasm/Binary/Section.hpp>
#include <Ab/Wasm/Binary/TypeCode.hpp>
#include <Pith/OStream.hpp>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

// https://github.com/WebAssembly/spec/tree/master/interpreter/#s-expression-syntax

namespace Ab {
namespace Wasm {
namespace Binary {

struct ModuleHeader {
	/// The magic constant at the beginning of every module.
	/// in LE, it's also the char array "\0asm".
	static constexpr std::uint32_t MAGIC = 0x6d736100;

	/// The WASM version. We only support 1.
	static constexpr std::uint32_t VERSION = 0x1;

	std::uint32_t magic;
	std::uint32_t version;
};

struct FunctionType {
	std::vector<TypeCode> paramTypes;
	bool hasReturnType;
	TypeCode returnType;
};

inline auto operator<<(Pith::SexprPrinter& out, const FunctionType& ft) -> Pith::SexprPrinter&;

struct GlobalType {
	TypeCode contentType;
	bool isMutable;
};

inline auto operator<<(Pith::SexprPrinter& out, const GlobalType& global) -> Pith::SexprPrinter&;

struct ResizableLimits {
	std::uint32_t initial;
	std::uint32_t max;
	bool hasMax;
};

inline auto operator<<(Pith::SexprPrinter& out, const ResizableLimits& limits)
	-> Pith::SexprPrinter& {
	out << limits.initial;
	if (limits.hasMax) {
		out << limits.max;
	}
	return out;
}

struct MemoryType {
	ResizableLimits limits;
};

inline auto operator<<(Pith::SexprPrinter& out, const MemoryType& type) -> Pith::SexprPrinter& {
	out << Pith::sexprStart << "memory" << type.limits << Pith::sexprEnd;
	return out;
}

struct TableType {
	TypeCode elementType;
	ResizableLimits limits;
};

inline auto operator<<(Pith::SexprPrinter& out, const TableType& type) -> Pith::SexprPrinter& {
	out << Pith::sexprStart << "table" << type.limits << type.elementType << Pith::sexprEnd;
	return out;
}

enum class ExternalKindCode : std::uint8_t { FUNCTION = 0, TABLE = 1, MEMORY = 2, GLOBAL = 3 };

inline auto tostring(ExternalKindCode kind) -> const char*;

inline auto operator<<(std::ostream& out, ExternalKindCode kind) -> std::ostream&;

struct ImportEntry {
	std::string module;
	std::string field;
	ExternalKindCode kind;
	union ImportEntryType {
		std::uint32_t function;
		GlobalType global;
		TableType table;
		MemoryType memory;
	} type;
};

inline auto operator<<(Pith::SexprPrinter& out, const ImportEntry& entry) -> Pith::SexprPrinter&;

struct Expression {
	OpCode op;
	union {
		std::uint32_t uint32;
		std::uint64_t uint64;
		std::int32_t int32;
		std::int64_t int64;
		float float32;
		double float64;
	} immediate;
};

inline auto operator<<(Pith::SexprPrinter& out, const Expression& expr) -> Pith::SexprPrinter& {
	out << expr.op;
	switch (expr.op) {
	case OpCode::GET_GLOBAL:
		out << expr.immediate.uint32;
		break;
	case OpCode::F32_CONST:
		out << expr.immediate.float32;
		break;
	case OpCode::F64_CONST:
		out << expr.immediate.float64;
		break;
	case OpCode::I64_CONST:
		out << expr.immediate.int64;
		break;
	case OpCode::I32_CONST:
		out << expr.immediate.int32;
		break;
	case OpCode::END:
	default:
		break;
	}
	return out;
}

struct ExportEntry {
	std::string field;
	ExternalKindCode kind;
	std::uint32_t index;
};

inline auto operator<<(Pith::SexprPrinter& out, const ExportEntry& entry) -> Pith::SexprPrinter&;

struct ElementEntry {
	std::uint32_t index;
	Expression offset;
	std::uint32_t elementCount;
};

struct LocalEntry {
	std::uint32_t count;
	TypeCode type;
};

inline auto operator<<(Pith::SexprPrinter& out, const LocalEntry& entry) -> Pith::SexprPrinter& {
	for (std::size_t i = 0; i < entry.count; i++) {
		out << entry.type;
	}
	return out;
}

struct FunctionBody {
	std::uint32_t bodySize;
	std::vector<LocalEntry> locals;
};

inline auto operator<<(Pith::SexprPrinter& out, const FunctionBody& body) -> Pith::SexprPrinter& {
	out << Pith::sexprStart << "local";
	for (const auto& local : body.locals) {
		out << local;
	}
	out  << Pith::sexprEnd;
	return out;
}

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#include <Ab/Wasm/Binary/Ir.inl.hpp>

#endif  // AB_WASM_BINARY_IR_HPP_
