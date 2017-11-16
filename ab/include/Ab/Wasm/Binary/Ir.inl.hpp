#ifndef AB_WASM_BINARY_IR_INL_HPP_
#define AB_WASM_BINARY_IR_INL_HPP_

#include <Ab/Wasm/Binary/Ir.hpp>

namespace Ab {
namespace Wasm {
namespace Binary {

inline auto operator<<(Pith::SexprPrinter& out, const FunctionType& ft) -> Pith::SexprPrinter& {
	out << Pith::freshLine;
	out << Pith::sexprStart;
	out << "func";
	if (ft.paramTypes.size() != 0) {
		out << Pith::sexprStart << "param";
		for (auto pt : ft.paramTypes)
			out << pt;
		out << Pith::sexprEnd;
	}
	if (ft.hasReturnType) {
		out << Pith::sexprStart << "result" << ft.returnType << Pith::sexprEnd;
	}
	out << Pith::sexprEnd;
	return out;
}

inline auto operator<<(Pith::SexprPrinter& out, const GlobalType& global) -> Pith::SexprPrinter& {
	out << Pith::sexprStart;
	if (global.isMutable) {
		out << "mut";
	}
	out << global.contentType;
	out << Pith::sexprEnd;
	return out;
}

inline auto toString(ExternalKindCode kind) -> const char* {
	switch (kind) {
	case ExternalKindCode::FUNCTION:
		return "func";
	case ExternalKindCode::TABLE:
		return "table";
	case ExternalKindCode::MEMORY:
		return "memory";
	case ExternalKindCode::GLOBAL:
		return "global";
	default:
		return "UNKNOWN_EXTERNAL_KIND_CODE";
	}
}

inline auto operator<<(std::ostream& out, ExternalKindCode kind) -> std::ostream& {
	return out << toString(kind);
}

inline auto operator<<(Pith::SexprPrinter& out, const ExportEntry& entry) -> Pith::SexprPrinter& {
	out << Pith::sexprStart;
	out << "export";
	out << Pith::stringify << entry.field;
	out << Pith::sexprStart << entry.kind << entry.index << Pith::sexprEnd;
	out << Pith::sexprEnd;
	return out;
}

inline auto operator<<(Pith::SexprPrinter& out, const ImportEntry& entry) -> Pith::SexprPrinter& {
	out << Pith::sexprStart;
	out << "import";
	out << Pith::stringify << entry.module;
	out << Pith::stringify << entry.field;
	out << "(;"
	    << "kind" << (std::uint32_t)entry.kind << ";)";

	switch (entry.kind) {
	case ExternalKindCode::FUNCTION:
		out << Pith::sexprStart << "type" << entry.type.function << Pith::sexprEnd;
		break;
	case ExternalKindCode::TABLE:
		out << entry.type.table;
		break;
	case ExternalKindCode::MEMORY:
		out << entry.type.memory;
		break;
	case ExternalKindCode::GLOBAL:
		out << entry.type.global;
		break;
	default:
		out << "UNKNOWN_EXTERNAL_KIND_CODE";
		break;
	}

	out << Pith::sexprEnd;
	return out;
}

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_BINARY_IR_INL_HPP_