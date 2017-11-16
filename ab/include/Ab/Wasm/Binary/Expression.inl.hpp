#ifndef AB_WASM_BINARY_EXPRESSION_INL_HPP_
#define AB_WASM_BINARY_EXPRESSION_INL_HPP_

#include <Ab/Wasm/Binary/Expression.hpp>

namespace Ab {
namespace Wasm {
namespace Binary {

inline AnyExpr::AnyExpr(OpCode op) : op_(op) {
}

inline auto AnyExpr::op() const -> OpCode {
	return op_;
}

inline auto AnyExpr::op(OpCode op) -> AnyExpr& {
	op_ = op;
	return *this;
}

template <typename Function>
inline auto ExprReader::operator()(std::istream& in, Function& function) -> void {
	OpCode op = OpCode::END;
	do {
		op = (OpCode)in.get();
		opDispatch<ReadExpr>(op, in, function);
	} while (op != OpCode::END);
}

inline ExprPrinter::ExprPrinter(Pith::SexprPrinter& out)
	: out_(out) {
}

template <typename Expr>
inline auto ExprPrinter::operator()(const Expr& e) -> void {
	out_ << Pith::freshLine << e;
}

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#endif // AB_WASM_BINARY_EXPRESSION_INL_HPP_