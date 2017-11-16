#ifndef AB_WASM_BINARY_EXPRESSION_HPP_
#define AB_WASM_BINARY_EXPRESSION_HPP_

#include <Ab/Config.hpp>
#include <Ab/Wasm/Binary/OpCode.hpp>
#include <Ab/leb128.hpp>
#include <Pith/SexprPrinter.hpp>
#include <cstddef>
#include <cstdint>
#// include <tuple>

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

struct Varuint32Immediate : Immediate {
	using Value = uint64_t;
	static auto read(std::istream& in, uint64_t& out) -> void {
		out = varuint32(in);
	}
};

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
	auto operator()(std::istream& in, NullaryExpr<op>& out) -> void {
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
	auto operator()(std::istream& in, UnaryExpr<op, Immediate>& out) -> void {
		Immediate::read(in, out.immediate());
	}
};

template<OpCode op, typename Immediate>
inline auto operator<<(Pith::SexprPrinter& out, const UnaryExpr<op, Immediate>& expr) -> Pith::SexprPrinter& {
	return out << OP_NAME<op> << expr.immediate();
}

template <OpCode op>
struct OpTraits {
	/// UnknownExpr is the default expression type for all op codes.
	using ExprType = NullaryExpr<op>;
};

using UnreachableExpr = NullaryExpr<OpCode::UNREACHABLE>;

template <>
struct OpTraits<OpCode::UNREACHABLE> {
	using ExprType = UnreachableExpr;
};

using NopExpr = NullaryExpr<OpCode::NOP>;

template <>
struct OpTraits<OpCode::NOP> {
	using ExprType = NopExpr;
};

///TODO: 
using BlockExpr = NullaryExpr<OpCode::BLOCK>;

template <>
struct OpTraits<OpCode::BLOCK> {
	using ExprType = BlockExpr;
};

using GetLocalExpr = UnaryExpr<OpCode::GET_LOCAL, Varuint32Immediate>;

template <>
struct OpTraits<OpCode::GET_LOCAL> {
	using ExprType = GetLocalExpr;
};

using SetLocalExpr = UnaryExpr<OpCode::SET_LOCAL, Varuint32Immediate>;

template <>
struct OpTraits<OpCode::SET_LOCAL> {
	using ExprType = SetLocalExpr;
};

using ReturnExpr = NullaryExpr<OpCode::RETURN>;

template <>
struct OpTraits<OpCode::RETURN> {
	using ExprType = ReturnExpr;
};


using EndExpr = NullaryExpr<OpCode::END>;

template <>
struct OpTraits<OpCode::END> {
	using ExprType = EndExpr;
};

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

inline auto operator<<(Pith::SexprPrinter& out, AnyExpr& any) -> Pith::SexprPrinter& {
	return exprDispatch(any, SexprPrint(), out);
}

/// Read the Expression that corresponds to the opcode, and call the function on expr.
template <OpCode op>
struct ReadExpr {
	/// Decode an expression and call function on the result.
	template <typename Function>
	auto operator()(std::istream& in, Function& function) -> void {
		using E = typename OpTraits<op>::ExprType;
		ReadImmediates<E> read;
		E expr;
		read(in, expr);
		function(expr);
	}
};

/// The WASM expression decoder.
struct ExprReader {
	/// Decode a sequence of expressions and call function on each element.
	template <typename Function>
	auto operator()(std::istream& in, Function& function) -> void;
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
};

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#include <Ab/Wasm/Binary/Expression.inl.hpp>

#endif  // AB_WASM_BINARY_EXPRESSION_HPP_
