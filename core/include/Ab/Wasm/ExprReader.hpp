#ifndef AB_WASM_EXPRREADER_HPP_
#define AB_WASM_EXPRREADER_HPP_

#include <Ab/Config.hpp>
#include <Ab/Wasm/ExprDispatch.hpp>
#include <Ab/Wasm/Expression.hpp>

namespace Ab {
namespace Wasm {

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
		-> void {
		OpCode op = OpCode::END;
		do {
			op = (OpCode)in.get();
			opDispatch<ReadExpr>(op, in, function, std::forward<Args>(args)...);
		} while (in.offset() < size);
	}
};

}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_EXPRREADER_HPP_
