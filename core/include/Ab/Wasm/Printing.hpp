#ifndef AB_WASM_PRINTING_HPP_
#define AB_WASM_PRINTING_HPP_

#include <Ab/Config.hpp>
#include <Ab/Sexpr.hpp>
#include <Ab/Wasm/Expression.hpp>
#include <Ab/Wasm/Ir.hpp>
#include <Ab/Wasm/OpCode.hpp>
#include <Ab/Wasm/ExprDispatch.hpp>

// https://github.com/WebAssembly/spec/tree/master/interpreter/#s-expression-syntax

namespace Ab::Wasm {

inline auto to_string(OpCode op) -> const char* { return opDispatch<OpName>(op); }

inline auto operator<<(Sexpr::Formatter& out, OpCode code) -> Sexpr::Formatter& {
	return out << to_string(code);
}

inline auto to_string(TypeCode code) -> const char* {
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
}

inline auto operator<<(Sexpr::Formatter& out, TypeCode code) -> Sexpr::Formatter& {
	return out << to_string(code);
}

inline auto to_string(SectionCode code) -> const char* {
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

inline auto operator<<(Sexpr::Formatter& out, SectionCode code) -> Sexpr::Formatter& {
	return out << to_string(code);
}

inline auto operator<<(Sexpr::Formatter& out, const Section& section) -> Sexpr::Formatter& {
	out << Sexpr::START << "section";
	out << Sexpr::START << "code" << section.code << Sexpr::END;

	if (section.code == SectionCode::NAME) {
		out << Sexpr::START << "name" << section.name << Sexpr::END;
	}

	out << Sexpr::START << "size" << section.length << Sexpr::END;

	out << Sexpr::END;

	return out;
}

inline auto operator<<(Sexpr::Formatter& out, const ResizableLimits& limits) -> Sexpr::Formatter& {
	out << limits.initial;
	if (limits.has_max) {
		out << limits.max;
	}
	return out;
}

inline auto operator<<(Sexpr::Formatter& out, const MemoryType& type) -> Sexpr::Formatter& {
	out << Sexpr::START << "memory" << type.limits << Sexpr::END;
	return out;
}

inline auto operator<<(Sexpr::Formatter& out, const TableType& type) -> Sexpr::Formatter& {
	out << Sexpr::START << "table" << type.limits << type.element_type << Sexpr::END;
	return out;
}

inline auto operator<<(Sexpr::Formatter& out, const FunctionType& ft) -> Sexpr::Formatter& {
	out << Sexpr::FRESH;
	out << Sexpr::START;
	out << "func";
	if (ft.param_types.size() != 0) {
		out << Sexpr::START << "param";
		for (auto pt : ft.param_types)
			out << pt;
		out << Sexpr::END;
	}
	if (ft.has_return_type) {
		out << Sexpr::START << "result" << ft.return_type << Sexpr::END;
	}
	out << Sexpr::END;
	return out;
}

inline auto operator<<(Sexpr::Formatter& out, const GlobalType& global) -> Sexpr::Formatter& {
	out << Sexpr::START;
	if (global.is_mutable) {
		out << "mut";
	}
	out << global.content_type;
	out << Sexpr::END;
	return out;
}

inline auto to_string(ExternalKindCode kind) -> const char* {
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

inline auto operator<<(Sexpr::Formatter& out, ExternalKindCode kind) -> Sexpr::Formatter& {
	return out << to_string(kind);
}

inline auto operator<<(Sexpr::Formatter& out, const ExportEntry& entry) -> Sexpr::Formatter& {
	out << Sexpr::START;
	out << "export";
	out << Sexpr::stringify(entry.field);
	out << Sexpr::START << entry.kind << entry.index << Sexpr::END;
	out << Sexpr::END;
	return out;
}

inline auto operator<<(Sexpr::Formatter& out, const ImportEntry& entry) -> Sexpr::Formatter& {
	out << Sexpr::START;
	out << "import";
	out << Sexpr::stringify(entry.module);
	out << Sexpr::stringify(entry.field);
	out << "(;"
	    << "kind" << (std::uint32_t)entry.kind << ";)";

	switch (entry.kind) {
	case ExternalKindCode::FUNCTION:
		out << Sexpr::START << "type" << "qqq" << entry.type.function << Sexpr::END;
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

	out << Sexpr::END;
	return out;
}

inline auto operator<<(Sexpr::Formatter& out, const LocalEntry& entry) -> Sexpr::Formatter& {
	for (std::size_t i = 0; i < entry.count; i++) {
		out << "test";
		out << entry.type;
	}
	return out;
}

inline auto operator<<(Sexpr::Formatter& out, const FunctionBody& body) -> Sexpr::Formatter& {
	fmt::print("!!! locals = {} !!!\n", body.locals.size());
	out << Sexpr::START << "local" << fmt::format("{}", body.locals.size());
	for (const auto& local : body.locals) {
		out << "tttt";
		out << local;
	}
	out << Sexpr::END;
	return out;
}

struct SexprFormat {
	/// Print to Sexpr::Formatter.
	template <typename Expr>
	auto operator()(const Expr& expr, Sexpr::Formatter& out) const -> Sexpr::Formatter& {
		return out << expr;
	}
};

/// Print to Sexpr::Formatter.
struct Print {
	template <typename T>
	auto operator()(T x, Sexpr::Formatter& out) const -> Sexpr::Formatter& {
		return out << x;
	}
};

/// Print to std::ostream.
struct PrintOp {
	template <typename Expr>
	auto operator()(const Expr&, std::ostream& out) const -> std::ostream& {
		return out << OP_NAME<Expr::OP_CODE>;
	}
};



template <typename Expr>
struct FormatExpr {
	void operator()(const Expr& expr, Sexpr::Formatter& fmt) {
		fmt << expr;
	}
};

inline auto operator<<(Sexpr::Formatter& fmt, const AnyExpr& any) -> Sexpr::Formatter& {
	dispatch<FormatExpr>(any, fmt);
	return fmt;
}

inline auto operator<<(Sexpr::Formatter& out, const BranchTableImmediate::Value& imm)
	-> Sexpr::Formatter& {
	out << "default" << imm.defaultTarget;
	for (const auto& target : imm.targetTable) {
		return out << target;
	}
	return out;
}

inline auto operator<<(Sexpr::Formatter& out, const MemoryImmediate::Value& immediate)
	-> Sexpr::Formatter& {
	out << immediate.flags << immediate.offset;
	return out;
}

template <OpCode op>
inline auto operator<<(std::ostream& out, const Expr<op>&) -> std::ostream& {
	return out << OP_NAME<op>;
}

template <OpCode op>
inline auto operator<<(Sexpr::Formatter& out, const NullaryExpr<op>&) -> Sexpr::Formatter& {
	return out << OP_NAME<op>;
}

template <OpCode op, typename Immediate>
inline auto operator<<(Sexpr::Formatter& out, const UnaryExpr<op, Immediate>& expr)
	-> Sexpr::Formatter& {
	return out << OP_NAME<op> << expr.immediate();
}

inline auto operator<<(Sexpr::Formatter& out, const InitExpr& expr) -> Sexpr::Formatter& {
	return out << expr.any;
}

inline auto operator<<(Sexpr::Formatter& out, const DataSegment& segment) -> Sexpr::Formatter& {
	out << Sexpr::START << "data" << segment.init_expr;
	std::string data = "\"";
	for (auto c : segment.data) {
		if (c > 32 && c < 127) {
			data += c;
		} else {
			data += '\\';
			data += (int)c;
		}
	}
	data += "\"";
	out << data << Sexpr::END;
	return out;
}

/// A WASM expression printer.
class ExprPrinter {
public:
	ExprPrinter(Sexpr::Formatter& out) : out_(out), block_depth_(0) {}

	/// Accepts any expr type, and prints it.
	template <typename Expr>
	auto operator()(const Expr& e) -> void {
		out_ << Sexpr::FRESH << e;
	}

	auto operator()(const IfExpr& e) -> void {
		out_ << Sexpr::FRESH << e;
		++out_.indent();
		++block_depth_;
	}

	auto operator()(const BlockExpr& e) -> void {
		/// Crappy inline validator.
		switch (e.immediate()) {
		case TypeCode::I32:
		case TypeCode::I64:
		case TypeCode::F32:
		case TypeCode::F64:
		case TypeCode::ANYFUNC:
		case TypeCode::FUNC:
		case TypeCode::EMPTY:
			break;
		default:
			throw std::runtime_error{"Bad typecode"};
		}
		out_ << Sexpr::FRESH << e;
		++out_.indent();
		++block_depth_;
	}

	auto operator()(const LoopExpr& e) -> void {
		out_ << Sexpr::FRESH << e;
		++out_.indent();
		++block_depth_;
	}

	auto operator()(const EndExpr& e) -> void {
		if (block_depth_ > 0) {
			--out_.indent();
			--block_depth_;
		}
		out_ << Sexpr::FRESH << e;
	}

	auto operator()(const ElseExpr& e) -> void {
		--out_.indent();
		out_ << Sexpr::FRESH << e;
		++out_.indent();
	}

private:
	Sexpr::Formatter& out_;
	std::size_t block_depth_;
};

}  // namespace Ab::Wasm

#endif  // AB_WASM_PRINTING_HPP_
