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

template <typename Function, typename... Args>
inline auto ExprReader::
operator()(ReaderInput& in, std::size_t size, Function&& function, Args&&... args) -> void {
	OpCode op = OpCode::END;
	do {
		op = (OpCode)in.get();
		opDispatch<ReadExpr>(op, in, function, std::forward<Args>(args)...);
	} while (in.offset() < size);
}

inline ExprPrinter::ExprPrinter(Pith::SexprPrinter& out) : out_(out), blockDepth_(0) {
}

template <typename Expr>
inline auto ExprPrinter::operator()(const Expr& e) -> void {
	out_ << Pith::freshLine << e;
}

template <>
inline auto ExprPrinter::operator()(const IfExpr& e) -> void {
	out_ << Pith::freshLine << e;
	blockDepth_++;
	out_.indent()++;
}

template <>
inline auto ExprPrinter::operator()(const EndExpr& e) -> void {
	if (blockDepth_ > 0) {
		--out_.indent();
		--blockDepth_;
	}
	out_ << Pith::freshLine << e;
}

template <>
inline auto ExprPrinter::operator()(const ElseExpr& e) -> void {
	--out_.indent();
	out_ << Pith::freshLine << e;
	++out_.indent();
}

template <>
inline auto ExprPrinter::operator()(const BlockExpr& e) -> void {
	out_ << Pith::freshLine << e;
	++out_.indent()++;
}

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_BINARY_EXPRESSION_INL_HPP_
