#ifndef AB_WASM_BINARY_EXPRESSION_HPP_
#define AB_WASM_BINARY_EXPRESSION_HPP_

#include <Ab/Config.hpp>
#include <Ab/Wasm/Binary/OpCode.hpp>
#include <Ab/Wasm/Binary/TypeCode.hpp>
#include <Ab/leb128.hpp>
#include <Pith/SexprPrinter.hpp>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace Ab {
namespace Wasm {
namespace Binary {

/// Any expression.
struct AnyExpr {
public:
	inline AnyExpr(OpCode op);

	inline auto op() const -> OpCode;

	inline auto op(OpCode op) -> AnyExpr&;

private:
	OpCode op_;
};

/// Print an expression. This will use the OpCode and opDispatch to cast itself to the accurate
/// expression before printing.
inline auto operator<<(Pith::SexprPrinter& out, const AnyExpr& any) -> Pith::SexprPrinter&;

/// Immediate Traits.
/// Member types:
///   ValueType
/// Member functions:
///   void read(std::istream& in, ValueType& out);
class Immediate {};

struct UnhandledImmediate : public Immediate {
	using Value = std::uintptr_t;
	static auto read(ReaderInput& in, std::uintptr_t& out) -> void {
		out = ~std::uintptr_t(0);
		throw std::runtime_error("Unhandled immediate");
	}
};

struct Varuint32Immediate : public Immediate {
	using Value = std::uint64_t;
	static auto read(ReaderInput& in, std::uint64_t& out) -> void {
		out = uleb128(in);
	}
};

struct SignatureImmediate : public Immediate {
	struct Value {
		std::uint32_t index;
		std::uint8_t reserved;
	};
	static auto read(ReaderInput& in, Value& out) -> void {
		out.index    = uleb128(in);  // varuint32
		out.reserved = uleb128(in);  // varuint1
	}
};

inline auto operator<<(Pith::SexprPrinter& out, const SignatureImmediate::Value& imm)
	-> Pith::SexprPrinter& {
	return out << imm.index << imm.reserved;
}

struct Varuint64Immediate : public Immediate {
	using Value = std::uint64_t;
	static auto read(ReaderInput& in, std::uint64_t& out) -> void {
		out = uleb128(in);
	}
};

struct Varint32Immediate : public Immediate {
	using Value = std::int64_t;
	static auto read(ReaderInput& in, std::int64_t& out) -> void {
		out = leb128(in);
	}
};

struct Varint64Immediate : public Immediate {
	using Value = std::int64_t;
	static auto read(ReaderInput& in, std::int64_t& out) -> void {
		out = leb128(in);
	}
};

struct Uint32Immediate : public Immediate {
	using Value = std::uint32_t;
	static auto read(ReaderInput& in, std::uint32_t& out) -> void {
		out = readNumber<std::uint32_t>(in);
	}
};

struct Uint64Immediate : public Immediate {
	using Value = std::uint64_t;
	static auto read(ReaderInput& in, std::uint64_t& out) -> void {
		out = readNumber<std::uint64_t>(in);
	}
};

struct Int32Immediate : public Immediate {
	using Value = std::int32_t;
	static auto read(ReaderInput& in, std::int32_t& out) -> void {
		out = readNumber<std::int32_t>(in);
	}
};

struct Int64Immediate : public Immediate {
	using Value = std::int64_t;
	static auto read(ReaderInput& in, std::int64_t& out) -> void {
		out = readNumber<std::int64_t>(in);
	}
};

struct TypeCodeImmediate : public Immediate {
	using Value = TypeCode;
	static auto read(ReaderInput& in, TypeCode& out) -> void {
		out = typeCode(in);
	}
};

struct BranchTableImmediate : public Immediate {
	struct Value {
		std::vector<std::uint32_t> targetTable;
		std::uint32_t defaultTarget;
	};

	static auto read(ReaderInput& in, Value& out) -> void {
		auto count = varuint32(in);
		out.targetTable.reserve(count);
		for (std::size_t i = 0; i < count; i++) {
			auto target = varuint32(in);
			out.targetTable.push_back(target);
		}
		out.defaultTarget = varuint32(in);
	}
};

inline auto operator<<(Pith::SexprPrinter& out, const BranchTableImmediate::Value& imm)
	-> Pith::SexprPrinter& {
	out << "default" << imm.defaultTarget;
	for (const auto& target : imm.targetTable) {
		return out << target;
	}
	return out;
}

struct MemoryImmediate : public Immediate {
	struct Value {
		std::uint64_t flags;
		std::uint64_t offset;
	};

	static auto read(ReaderInput& in, Value& out) -> void {
		out.flags  = varuint32(in);
		out.offset = varuint32(in);
	}
};

inline auto operator<<(Pith::SexprPrinter& out, const MemoryImmediate::Value& immediate)
	-> Pith::SexprPrinter& {
	out << immediate.flags << immediate.offset;
	return out;
}

/// An expression type with a known opcode value.
template <OpCode op_v>
class Expr : public AnyExpr {
public:
	static constexpr OpCode OP = op_v;
	Expr() : AnyExpr(op_v) {
	}
};

template <typename Expr>
struct ReadImmediates;

template <OpCode op>
inline auto operator<<(std::ostream& out, const Expr<op>& expr) -> std::ostream& {
	return out << OP_NAME<op>;
}

template <OpCode op>
struct NullaryExpr : public Expr<op> {};

template <OpCode op>
struct ReadImmediates<NullaryExpr<op>> {
	auto operator()(ReaderInput& in, NullaryExpr<op>& out) -> void {
		// Nullary expressions have no immediates, so there is no decoding work here.
	}
};

template <OpCode op>
inline auto operator<<(Pith::SexprPrinter& out, const NullaryExpr<op>& expr)
	-> Pith::SexprPrinter& {
	return out << OP_NAME<op>;
}

template <OpCode op_v, typename Immediate>
struct UnaryExpr : public Expr<op_v> {
public:
	using Value = typename Immediate::Value;

	inline auto immediate() -> Value& {
		return immediate_;
	}

	inline auto immediate() const -> const Value& {
		return immediate_;
	}

	inline auto immediate(const Value& immediate) -> UnaryExpr<op_v, Immediate>& {
		immediate_ = immediate;
		return *this;
	}

private:
	Value immediate_;
};

template <OpCode op, typename Immediate>
struct ReadImmediates<UnaryExpr<op, Immediate>> {
	auto operator()(ReaderInput& in, UnaryExpr<op, Immediate>& out) -> void {
		Immediate::read(in, out.immediate());
	}
};

template <OpCode op, typename Immediate>
inline auto operator<<(Pith::SexprPrinter& out, const UnaryExpr<op, Immediate>& expr)
	-> Pith::SexprPrinter& {
	return out << OP_NAME<op> << expr.immediate();
}

template <typename E>
struct ExprMap {
	using ExprType = E;
};

template <OpCode op>
struct OpTraits {
	/// UnknownExpr is the default expression type for all op codes.
	using ExprType = NullaryExpr<op>;
};

/// unreachable

using UnreachableExpr = NullaryExpr<OpCode::UNREACHABLE>;

template <>
struct OpTraits<OpCode::UNREACHABLE> {
	using ExprType = UnreachableExpr;
};

/// nop

using NopExpr = NullaryExpr<OpCode::NOP>;

template <>
struct OpTraits<OpCode::NOP> {
	using ExprType = NopExpr;
};

/// block

using BlockExpr = UnaryExpr<OpCode::BLOCK, TypeCodeImmediate>;

template <>
struct OpTraits<OpCode::BLOCK> {
	using ExprType = BlockExpr;
};

/// if

using IfExpr = UnaryExpr<OpCode::IF, TypeCodeImmediate>;

template <>
struct OpTraits<OpCode::IF> {
	using ExprType = IfExpr;
};

/// else

using ElseExpr = NullaryExpr<OpCode::ELSE>;

template <>
struct OpTraits<OpCode::ELSE> {
	using ExprType = ElseExpr;
};

/// end

using EndExpr = NullaryExpr<OpCode::END>;

template <>
struct OpTraits<OpCode::END> {
	using ExprType = EndExpr;
};

/// br

using BrExpr = UnaryExpr<OpCode::BR, Varuint32Immediate>;

template <>
struct OpTraits<OpCode::BR> {
	using ExprType = BrExpr;
};

/// br_if

using BrIfExpr = UnaryExpr<OpCode::BR_IF, Varuint32Immediate>;

template <>
struct OpTraits<OpCode::BR_IF> {
	using ExprType = BrIfExpr;
};

/// br_table

using BrTableExpr = UnaryExpr<OpCode::BR_TABLE, BranchTableImmediate>;

template <>
struct OpTraits<OpCode::BR_TABLE> {
	using ExprType = BrTableExpr;
};

/// return

using ReturnExpr = NullaryExpr<OpCode::RETURN>;

template <>
struct OpTraits<OpCode::RETURN> {
	using ExprType = ReturnExpr;
};

/// call

using CallExpr = UnaryExpr<OpCode::CALL, Varuint32Immediate>;

template <>
struct OpTraits<OpCode::CALL> {
	using ExprType = CallExpr;
};

/// call_indirect

using CallIndirectExpr = UnaryExpr<OpCode::CALL_INDIRECT, SignatureImmediate>;

template <>
struct OpTraits<OpCode::CALL_INDIRECT> {
	using ExprType = CallIndirectExpr;
};

/// drop

using DropExpr = NullaryExpr<OpCode::DROP>;

template <>
struct OpTraits<OpCode::DROP> {
	using ExprType = DropExpr;
};

/// select

using SelectExpr = NullaryExpr<OpCode::SELECT>;

template <>
struct OpTraits<OpCode::SELECT> {
	using ExprType = SelectExpr;
};

/// get_local

using GetLocalExpr = UnaryExpr<OpCode::GET_LOCAL, Varuint32Immediate>;

template <>
struct OpTraits<OpCode::GET_LOCAL> {
	using ExprType = GetLocalExpr;
};

/// set_local

using SetLocalExpr = UnaryExpr<OpCode::SET_LOCAL, Varuint32Immediate>;

template <>
struct OpTraits<OpCode::SET_LOCAL> {
	using ExprType = SetLocalExpr;
};

/// tee_local

using TeeLocalExpr = UnaryExpr<OpCode::TEE_LOCAL, Varuint32Immediate>;

template <>
struct OpTraits<OpCode::TEE_LOCAL> {
	using ExprType = TeeLocalExpr;
};

/// get_local

using GetGlobalExpr = UnaryExpr<OpCode::GET_GLOBAL, Varuint32Immediate>;

template <>
struct OpTraits<OpCode::GET_GLOBAL> {
	using ExprType = GetGlobalExpr;
};

/// set_local

using SetGlobalExpr = UnaryExpr<OpCode::SET_GLOBAL, Varuint32Immediate>;

template <>
struct OpTraits<OpCode::SET_GLOBAL> {
	using ExprType = SetGlobalExpr;
};

/// load

using I32LoadExpr = UnaryExpr<OpCode::I32_LOAD, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I32_LOAD> {
	using ExprType = I32LoadExpr;
};

using I64LoadExpr = UnaryExpr<OpCode::I64_LOAD, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD> {
	using ExprType = I64LoadExpr;
};

using F32LoadExpr = UnaryExpr<OpCode::F32_LOAD, MemoryImmediate>;

template <>
struct OpTraits<OpCode::F32_LOAD> {
	using ExprType = F32LoadExpr;
};

using F64LoadExpr = UnaryExpr<OpCode::F64_LOAD, MemoryImmediate>;

template <>
struct OpTraits<OpCode::F64_LOAD> {
	using ExprType = F64LoadExpr;
};

using I32Load8sExpr = UnaryExpr<OpCode::I32_LOAD8_S, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I32_LOAD8_S> {
	using ExprType = I32Load8sExpr;
};

using I32Load8uExpr = UnaryExpr<OpCode::I32_LOAD8_U, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I32_LOAD8_U> {
	using ExprType = I32Load8uExpr;
};

using I32Load16sExpr = UnaryExpr<OpCode::I32_LOAD16_S, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I32_LOAD16_S> {
	using ExprType = I32Load16sExpr;
};

using I32Load16uExpr = UnaryExpr<OpCode::I32_LOAD16_U, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I32_LOAD16_U> {
	using ExprType = I32Load16uExpr;
};

using I64Load8sExpr = UnaryExpr<OpCode::I64_LOAD8_S, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD8_S> {
	using ExprType = I64Load8sExpr;
};

using I64Load8uExpr = UnaryExpr<OpCode::I64_LOAD8_U, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD8_U> {
	using ExprType = I64Load8uExpr;
};

using I64Load16sExpr = UnaryExpr<OpCode::I64_LOAD16_S, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD16_S> {
	using ExprType = I64Load16sExpr;
};

using I64Load16uExpr = UnaryExpr<OpCode::I64_LOAD16_U, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD16_U> {
	using ExprType = I64Load16uExpr;
};

using I64Load32sExpr = UnaryExpr<OpCode::I64_LOAD32_S, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD32_S> {
	using ExprType = I64Load32sExpr;
};

using I64Load32uExpr = UnaryExpr<OpCode::I64_LOAD32_U, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD32_U> {
	using ExprType = I64Load32uExpr;
};

/// TODO: Store Expressions

/// TODO: Memory Expressions

/// Constants

using I32ConstExpr = UnaryExpr<OpCode::I32_CONST, Varint32Immediate>;

template <>
struct OpTraits<OpCode::I32_CONST> {
	using ExprType = I32ConstExpr;
};

using I64ConstExpr = UnaryExpr<OpCode::I64_CONST, Varint64Immediate>;

template <>
struct OpTraits<OpCode::I64_CONST> {
	using ExprType = I64ConstExpr;
};

using F32ConstExpr = UnaryExpr<OpCode::F32_CONST, Uint32Immediate>;

template <>
struct OpTraits<OpCode::F32_CONST> {
	using ExprType = F32ConstExpr;
};

using F64ConstExpr = UnaryExpr<OpCode::F64_CONST, Uint64Immediate>;

template <>
struct OpTraits<OpCode::F64_CONST> {
	using ExprType = F64ConstExpr;
};

/// loop

using LoopExpr = UnaryExpr<OpCode::LOOP, TypeCodeImmediate>;

template <>
struct OpTraits<OpCode::LOOP> {
	using ExprType = LoopExpr;
};

/// if

// using IfExpr = UnaryExpr<OpCode::

#if 0

/// Map OpCodes to Expression types.
template <OpCode op>
using ExprType = UnknownExpr;

template <>
using ExprType<OpCode::UNREACHABLE> = Unreachable;

inline auto operator<<(Pith::SexprPrinter& out, Unreachable& expr) -> Pith::SexprPrinter& {
	return out << "unreachable";
}

struct Nop : public Nullary {
	Nop() : Nullary(OpCode::NOP) {
	}
};

inline auto operator<<(Pith::SexprPrinter& out, Unreachable& expr) -> Pith::SexprPrinter& {
	return out << "unreachable";
}

struct Loop : public Unary {
	Loop() : Unary(OpCode::LOOP) {
	}
};

struct GetLocal : public Unary {
	GetLocal(std::uint32_t index) : Base
};

#endif

/// A functor that will cast the AnyExpr to a concrete expression type, and call function(expr,
/// args...);
//// The expression type is obtained as OpTraits<Op>::Expr.
template <OpCode op>
struct ExprCastDispatch {
	///
	template <typename Function, typename... Args>
	auto operator()(Function&& function, AnyExpr& expr, Args&&... args)
		-> decltype(function(expr, std::forward<Args>(args)...)) {
		using Expr = typename OpTraits<op>::ExprType;
		return function(static_cast<Expr&>(expr), std::forward<Args>(args)...);
	}

	template <typename Function, typename... Args>
	auto operator()(Function&& function, const AnyExpr& expr, Args&&... args)
		-> decltype(function(expr, std::forward<Args>(args)...)) {
		using Expr = typename OpTraits<op>::ExprType;
		return function(static_cast<const Expr&>(expr), std::forward<Args>(args)...);
	}
};

/// Apply a function to an expression.
/// This function will downcast the AnyExpr to the concrete expression type, calling function on it.
/// The dispatcher will look at the opcode, and dispatch to the function.
/// This dispatch function performs run-time dispatching. For generating switch statements, or your
/// own runtime dispatch, see opDispatch.
template <typename Function, typename... Args>
inline auto exprDispatch(AnyExpr& expr, Function&& function, Args&&... args)
	-> decltype(function(std::declval<AnyExpr>(), args...)) {
	return opDispatch<ExprCastDispatch>(expr.op(), function, expr, std::forward<Args>(args)...);
}

template <typename Function, typename... Args>
inline auto exprDispatch(const AnyExpr& expr, Function&& function, Args&&... args)
	-> decltype(function(std::declval<AnyExpr>(), args...)) {
	return opDispatch<ExprCastDispatch>(expr.op(), function, expr, std::forward<Args>(args)...);
}

/// Print to std::ostream.
struct PrintOp {
	template <typename Expr>
	auto operator()(const Expr& x, std::ostream& out) const -> std::ostream& {
		return out << OP_NAME<Expr::OP_CODE>;
	}
};

/// Print to std::ostream.
struct Print {
	template <typename T>
	auto operator()(T x, std::ostream& out) const -> std::ostream& {
		return out << x;
	}
};

struct SexprPrint {
	/// Print to SexprPrinter.
	template <typename Expr>
	auto operator()(const Expr& expr, Pith::SexprPrinter& out) const -> Pith::SexprPrinter& {
		return out << expr;
	}
};

inline auto operator<<(Pith::SexprPrinter& out, const AnyExpr& any) -> Pith::SexprPrinter& {
	return exprDispatch(any, SexprPrint(), out);
}

/// Read the Expression that corresponds to the opcode, and call the function on expr.
template <OpCode op>
struct ReadExpr {
	/// Decode an expression and call function on the result.
	template <typename Function, typename... Args>
	auto operator()(ReaderInput& in, Function&& function, Args&&... args) -> void {
		using E = typename OpTraits<op>::ExprType;
		ReadImmediates<E> read;
		E expr;
		read(in, expr);
		function(expr, std::forward<Args>(args)...);
	}
};

/// The WASM expression decoder.
struct ExprReader {
	/// Decode a sequence of expressions and call function on each expression.
	template <typename Function, typename... Args>
	auto operator()(ReaderInput& in, std::size_t size, Function&& function, Args&&... args)
		-> void;
};

/// A WASM expression printer.
class ExprPrinter {
public:
	inline ExprPrinter(Pith::SexprPrinter& out);

	/// Accepts any expr type, and prints it.
	template <typename Expr>
	inline auto operator()(const Expr& e) -> void;

private:
	Pith::SexprPrinter& out_;
	std::size_t blockDepth_;
};

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#include <Ab/Wasm/Binary/Expression.inl.hpp>

#endif  // AB_WASM_BINARY_EXPRESSION_HPP_
