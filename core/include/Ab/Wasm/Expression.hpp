#ifndef AB_WASM_BINARY_EXPRESSION_HPP_
#define AB_WASM_BINARY_EXPRESSION_HPP_

#include <Ab/Config.hpp>
#include <Ab/Wasm/OpCode.hpp>
#include <Ab/Wasm/TypeCode.hpp>
#include <Ab/leb128.hpp>
#include <Ab/Sexpr.hpp>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace Ab {
namespace Wasm {

inline auto typeCode(ReaderInput& in) -> Ab::Wasm::TypeCode {
	return (Ab::Wasm::TypeCode)in.get();
}

/// Any expression.
struct AnyExpr {
public:
	inline AnyExpr(OpCode op) : op_(op) {}

	inline auto op() const -> OpCode { return op_; }

	inline auto op(OpCode op) -> AnyExpr& { op_ = op; return *this; }

private:
	OpCode op_;
};

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

inline auto operator<<(Sexpr::Formatter& out, const SignatureImmediate::Value& imm) -> Sexpr::Formatter& {
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
struct NullaryExpr : public Expr<op> {};

template <OpCode op>
struct ReadImmediates<NullaryExpr<op>> {
	auto operator()(ReaderInput& in, NullaryExpr<op>& out) -> void {
		// Nullary expressions have no immediates, so there is no decoding work here.
	}
};

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

template <OpCode op>
struct OpTraits {
// 	/// UnknownExpr is the default expression type for all op codes.
// #ifdef AB_DEBUG
// 	using ExprType = UnaryExpr<op, UnhandledImmediate>;
// #else
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

/// get_global

using GetGlobalExpr = UnaryExpr<OpCode::GET_GLOBAL, Varuint32Immediate>;

template <>
struct OpTraits<OpCode::GET_GLOBAL> {
	using ExprType = GetGlobalExpr;
};

/// set_global

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

using I32Load8SExpr = UnaryExpr<OpCode::I32_LOAD8_S, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I32_LOAD8_S> {
	using ExprType = I32Load8SExpr;
};

using I32Load8UExpr = UnaryExpr<OpCode::I32_LOAD8_U, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I32_LOAD8_U> {
	using ExprType = I32Load8UExpr;
};

using I32Load16SExpr = UnaryExpr<OpCode::I32_LOAD16_S, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I32_LOAD16_S> {
	using ExprType = I32Load16SExpr;
};

using I32Load16UExpr = UnaryExpr<OpCode::I32_LOAD16_U, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I32_LOAD16_U> {
	using ExprType = I32Load16UExpr;
};

using I64Load8SExpr = UnaryExpr<OpCode::I64_LOAD8_S, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD8_S> {
	using ExprType = I64Load8SExpr;
};

using I64Load8UExpr = UnaryExpr<OpCode::I64_LOAD8_U, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD8_U> {
	using ExprType = I64Load8UExpr;
};

using I64Load16SExpr = UnaryExpr<OpCode::I64_LOAD16_S, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD16_S> {
	using ExprType = I64Load16SExpr;
};

using I64Load16UExpr = UnaryExpr<OpCode::I64_LOAD16_U, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD16_U> {
	using ExprType = I64Load16UExpr;
};

using I64Load32SExpr = UnaryExpr<OpCode::I64_LOAD32_S, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD32_S> {
	using ExprType = I64Load32SExpr;
};

using I64Load32UExpr = UnaryExpr<OpCode::I64_LOAD32_U, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_LOAD32_U> {
	using ExprType = I64Load32UExpr;
};

/// TODO: Store Expressions

using I32StoreExpr = UnaryExpr<OpCode::I32_STORE, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I32_STORE> {
	using ExprType = I32StoreExpr;
};

using I64StoreExpr = UnaryExpr<OpCode::I64_STORE, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_STORE> {
	using ExprType = I64StoreExpr;
};

using F32StoreExpr = UnaryExpr<OpCode::F32_STORE, MemoryImmediate>;

template <>
struct OpTraits<OpCode::F32_STORE> {
	using ExprType = F32StoreExpr;
};

using F64StoreExpr = UnaryExpr<OpCode::F64_STORE, MemoryImmediate>;

template <>
struct OpTraits<OpCode::F64_STORE> {
	using ExprType = F64StoreExpr;
};

using I32Store8Expr = UnaryExpr<OpCode::I32_STORE8, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I32_STORE8> {
	using ExprType = I32Store8Expr;
};

using I32Store16Expr = UnaryExpr<OpCode::I32_STORE16, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I32_STORE16> {
	using ExprType = I32Store16Expr;
};

using I64Store8Expr = UnaryExpr<OpCode::I64_STORE8, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_STORE8> {
	using ExprType = I64Store8Expr;
};

using I64Store16Expr = UnaryExpr<OpCode::I64_STORE16, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_STORE16> {
	using ExprType = I64Store16Expr;
};

using I64Store32Expr = UnaryExpr<OpCode::I64_STORE32, MemoryImmediate>;

template <>
struct OpTraits<OpCode::I64_STORE32> {
	using ExprType = I64Store32Expr;
};

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



	/// Comparison Operators
	/// No Immediates


using I32EqzExpr = NullaryExpr<OpCode::I32_EQZ>;

template <>
struct OpTraits<OpCode::I32_EQZ> {
	using ExprType = I32EqzExpr;
};

using I32EqExpr = NullaryExpr<OpCode::I32_EQ>;

template <>
struct OpTraits<OpCode::I32_EQ> {
	using ExprType = I32EqExpr;
};

using I32NeExpr = NullaryExpr<OpCode::I32_NE>;

template <>
struct OpTraits<OpCode::I32_NE> {
	using ExprType = I32NeExpr;
};

using I32LtSExpr = NullaryExpr<OpCode::I32_LT_S>;

template <>
struct OpTraits<OpCode::I32_LT_S> {
	using ExprType = I32LtSExpr;
};

using I32LtUExpr = NullaryExpr<OpCode::I32_LT_U>;

template <>
struct OpTraits<OpCode::I32_LT_U> {
	using ExprType = I32LtUExpr;
};

using I32GtSExpr = NullaryExpr<OpCode::I32_GT_S>;

template <>
struct OpTraits<OpCode::I32_GT_S> {
	using ExprType = I32GtSExpr;
};

using I32GtUExpr = NullaryExpr<OpCode::I32_GT_U>;

template <>
struct OpTraits<OpCode::I32_GT_U> {
	using ExprType = I32GtUExpr;
};

using I32LeSExpr = NullaryExpr<OpCode::I32_LE_S>;

template <>
struct OpTraits<OpCode::I32_LE_S> {
	using ExprType = I32LeSExpr;
};

using I32LeUExpr = NullaryExpr<OpCode::I32_LE_U>;

template <>
struct OpTraits<OpCode::I32_LE_U> {
	using ExprType = I32LeUExpr;
};

using I32GeSExpr = NullaryExpr<OpCode::I32_GE_S>;

template <>
struct OpTraits<OpCode::I32_GE_S> {
	using ExprType = I32GeSExpr;
};

using I32GeUExpr = NullaryExpr<OpCode::I32_GE_U>;

template <>
struct OpTraits<OpCode::I32_GE_U> {
	using ExprType = I32GeUExpr;
};

using I64EqzExpr = NullaryExpr<OpCode::I64_EQZ>;

template <>
struct OpTraits<OpCode::I64_EQZ> {
	using ExprType = I64EqzExpr;
};

using I64EqExpr = NullaryExpr<OpCode::I64_EQ>;

template <>
struct OpTraits<OpCode::I64_EQ> {
	using ExprType = I64EqExpr;
};

using I64NeExpr = NullaryExpr<OpCode::I64_NE>;

template <>
struct OpTraits<OpCode::I64_NE> {
	using ExprType = I64NeExpr;
};

using I64LtSExpr = NullaryExpr<OpCode::I64_LT_S>;

template <>
struct OpTraits<OpCode::I64_LT_S> {
	using ExprType = I64LtSExpr;
};

using I64LtUExpr = NullaryExpr<OpCode::I64_LT_U>;

template <>
struct OpTraits<OpCode::I64_LT_U> {
	using ExprType = I64LtUExpr;
};

using I64GtSExpr = NullaryExpr<OpCode::I64_GT_S>;

template <>
struct OpTraits<OpCode::I64_GT_S> {
	using ExprType = I64GtSExpr;
};

using I64GtUExpr = NullaryExpr<OpCode::I64_GT_U>;

template <>
struct OpTraits<OpCode::I64_GT_U> {
	using ExprType = I64GtUExpr;
};

using I64LeSExpr = NullaryExpr<OpCode::I64_LE_S>;

template <>
struct OpTraits<OpCode::I64_LE_S> {
	using ExprType = I64LeSExpr;
};

using I64LeUExpr = NullaryExpr<OpCode::I64_LE_U>;

template <>
struct OpTraits<OpCode::I64_LE_U> {
	using ExprType = I64LeUExpr;
};

using I64GeSExpr = NullaryExpr<OpCode::I64_GE_S>;

template <>
struct OpTraits<OpCode::I64_GE_S> {
	using ExprType = I64GeSExpr;
};

using I64GeUExpr = NullaryExpr<OpCode::I64_GE_U>;

template <>
struct OpTraits<OpCode::I64_GE_U> {
	using ExprType = I64GeUExpr;
};;

using F32EqExpr = NullaryExpr<OpCode::F32_EQ>;

template <>
struct OpTraits<OpCode::F32_EQ> {
	using ExprType = F32EqExpr;
};

using F32NeExpr = NullaryExpr<OpCode::F32_NE>;

template <>
struct OpTraits<OpCode::F32_NE> {
	using ExprType = F32NeExpr;
};

using F32LtExpr = NullaryExpr<OpCode::F32_LT>;

template <>
struct OpTraits<OpCode::F32_LT> {
	using ExprType = F32LtExpr;
};

using F32GtExpr = NullaryExpr<OpCode::F32_GT>;

template <>
struct OpTraits<OpCode::F32_GT> {
	using ExprType = F32GtExpr;
};

using F32LeExpr = NullaryExpr<OpCode::F32_LE>;

template <>
struct OpTraits<OpCode::F32_LE> {
	using ExprType = F32LeExpr;
};

using F32GeExpr = NullaryExpr<OpCode::F32_GE>;

template <>
struct OpTraits<OpCode::F32_GE> {
	using ExprType = F32GeExpr;
};

using F64EqExpr = NullaryExpr<OpCode::F64_EQ>;

template <>
struct OpTraits<OpCode::F64_EQ> {
	using ExprType = F64EqExpr;
};

using F64NeExpr = NullaryExpr<OpCode::F64_NE>;

template <>
struct OpTraits<OpCode::F64_NE> {
	using ExprType = F64NeExpr;
};

using F64LtExpr = NullaryExpr<OpCode::F64_LT>;

template <>
struct OpTraits<OpCode::F64_LT> {
	using ExprType = F64LtExpr;
};

using F64GtExpr = NullaryExpr<OpCode::F64_GT>;

template <>
struct OpTraits<OpCode::F64_GT> {
	using ExprType = F64GtExpr;
};

using F64LeExpr = NullaryExpr<OpCode::F64_LE>;

template <>
struct OpTraits<OpCode::F64_LE> {
	using ExprType = F64LeExpr;
};

using F64GeExpr = NullaryExpr<OpCode::F64_GE>;

template <>
struct OpTraits<OpCode::F64_GE> {
	using ExprType = F64GeExpr;
};

using I32ClzExpr = NullaryExpr<OpCode::I32_CLZ>;

template <>
struct OpTraits<OpCode::I32_CLZ> {
	using ExprType = I32ClzExpr;
};

using I32CtzExpr = NullaryExpr<OpCode::I32_CTZ>;

template <>
struct OpTraits<OpCode::I32_CTZ> {
	using ExprType = I32CtzExpr;
};

using I32PopcntExpr = NullaryExpr<OpCode::I32_POPCNT>;

template <>
struct OpTraits<OpCode::I32_POPCNT> {
	using ExprType = I32PopcntExpr;
};

using I32AddExpr = NullaryExpr<OpCode::I32_ADD>;

template <>
struct OpTraits<OpCode::I32_ADD> {
	using ExprType = I32AddExpr;
};

using I32SubExpr = NullaryExpr<OpCode::I32_SUB>;

template <>
struct OpTraits<OpCode::I32_SUB> {
	using ExprType = I32SubExpr;
};

using I32MulExpr = NullaryExpr<OpCode::I32_MUL>;

template <>
struct OpTraits<OpCode::I32_MUL> {
	using ExprType = I32MulExpr;
};

using I32DivSExpr = NullaryExpr<OpCode::I32_DIV_S>;

template <>
struct OpTraits<OpCode::I32_DIV_S> {
	using ExprType = I32DivSExpr;
};

using I32DivUExpr = NullaryExpr<OpCode::I32_DIV_U>;

template <>
struct OpTraits<OpCode::I32_DIV_U> {
	using ExprType = I32DivUExpr;
};

using I32RemSExpr = NullaryExpr<OpCode::I32_REM_S>;

template <>
struct OpTraits<OpCode::I32_REM_S> {
	using ExprType = I32RemSExpr;
};

using I32RemUExpr = NullaryExpr<OpCode::I32_REM_U>;

template <>
struct OpTraits<OpCode::I32_REM_U> {
	using ExprType = I32RemUExpr;
};

using I32AndExpr = NullaryExpr<OpCode::I32_AND>;

template <>
struct OpTraits<OpCode::I32_AND> {
	using ExprType = I32AndExpr;
};

using I32OrExpr = NullaryExpr<OpCode::I32_OR>;

template <>
struct OpTraits<OpCode::I32_OR> {
	using ExprType = I32OrExpr;
};

using I32XorExpr = NullaryExpr<OpCode::I32_XOR>;

template <>
struct OpTraits<OpCode::I32_XOR> {
	using ExprType = I32XorExpr;
};

using I32ShlExpr = NullaryExpr<OpCode::I32_SHL>;

template <>
struct OpTraits<OpCode::I32_SHL> {
	using ExprType = I32ShlExpr;
};

using I32ShrSExpr = NullaryExpr<OpCode::I32_SHR_S>;

template <>
struct OpTraits<OpCode::I32_SHR_S> {
	using ExprType = I32ShrSExpr;
};

using I32ShrUExpr = NullaryExpr<OpCode::I32_SHR_U>;

template <>
struct OpTraits<OpCode::I32_SHR_U> {
	using ExprType = I32ShrUExpr;
};

using I32RotlExpr = NullaryExpr<OpCode::I32_ROTL>;

template <>
struct OpTraits<OpCode::I32_ROTL> {
	using ExprType = I32RotlExpr;
};

using I32RotrExpr = NullaryExpr<OpCode::I32_ROTR>;

template <>
struct OpTraits<OpCode::I32_ROTR> {
	using ExprType = I32RotrExpr;
};

using I64ClzExpr = NullaryExpr<OpCode::I64_CLZ>;

template <>
struct OpTraits<OpCode::I64_CLZ> {
	using ExprType = I64ClzExpr;
};

using I64CtzExpr = NullaryExpr<OpCode::I64_CTZ>;

template <>
struct OpTraits<OpCode::I64_CTZ> {
	using ExprType = I64CtzExpr;
};

using I64PopcntExpr = NullaryExpr<OpCode::I64_POPCNT>;

template <>
struct OpTraits<OpCode::I64_POPCNT> {
	using ExprType = I64PopcntExpr;
};

using I64AddExpr = NullaryExpr<OpCode::I64_ADD>;

template <>
struct OpTraits<OpCode::I64_ADD> {
	using ExprType = I64AddExpr;
};

using I64SubExpr = NullaryExpr<OpCode::I64_SUB>;

template <>
struct OpTraits<OpCode::I64_SUB> {
	using ExprType = I64SubExpr;
};

using I64MulExpr = NullaryExpr<OpCode::I64_MUL>;

template <>
struct OpTraits<OpCode::I64_MUL> {
	using ExprType = I64MulExpr;
};

using I64DivSExpr = NullaryExpr<OpCode::I64_DIV_S>;

template <>
struct OpTraits<OpCode::I64_DIV_S> {
	using ExprType = I64DivSExpr;
};

using I64DivUExpr = NullaryExpr<OpCode::I64_DIV_U>;

template <>
struct OpTraits<OpCode::I64_DIV_U> {
	using ExprType = I64DivUExpr;
};

using I64RemSExpr = NullaryExpr<OpCode::I64_REM_S>;

template <>
struct OpTraits<OpCode::I64_REM_S> {
	using ExprType = I64RemSExpr;
};

using I64RemUExpr = NullaryExpr<OpCode::I64_REM_U>;

template <>
struct OpTraits<OpCode::I64_REM_U> {
	using ExprType = I64RemUExpr;
};

using I64AndExpr = NullaryExpr<OpCode::I64_AND>;

template <>
struct OpTraits<OpCode::I64_AND> {
	using ExprType = I64AndExpr;
};

using I64OrExpr = NullaryExpr<OpCode::I64_OR>;

template <>
struct OpTraits<OpCode::I64_OR> {
	using ExprType = I64OrExpr;
};

using I64XorExpr = NullaryExpr<OpCode::I64_XOR>;

template <>
struct OpTraits<OpCode::I64_XOR> {
	using ExprType = I64XorExpr;
};

using I64ShlExpr = NullaryExpr<OpCode::I64_SHL>;

template <>
struct OpTraits<OpCode::I64_SHL> {
	using ExprType = I64ShlExpr;
};

using I64ShrSExpr = NullaryExpr<OpCode::I64_SHR_S>;

template <>
struct OpTraits<OpCode::I64_SHR_S> {
	using ExprType = I64ShrSExpr;
};

using I64ShrUExpr = NullaryExpr<OpCode::I64_SHR_U>;

template <>
struct OpTraits<OpCode::I64_SHR_U> {
	using ExprType = I64ShrUExpr;
};

using I64RotlExpr = NullaryExpr<OpCode::I64_ROTL>;

template <>
struct OpTraits<OpCode::I64_ROTL> {
	using ExprType = I64RotlExpr;
};

using I64RotrExpr = NullaryExpr<OpCode::I64_ROTR>;

template <>
struct OpTraits<OpCode::I64_ROTR> {
	using ExprType = I64RotrExpr;
};

using F32AbsExpr = NullaryExpr<OpCode::F32_ABS>;

template <>
struct OpTraits<OpCode::F32_ABS> {
	using ExprType = F32AbsExpr;
};

using F32NegExpr = NullaryExpr<OpCode::F32_NEG>;

template <>
struct OpTraits<OpCode::F32_NEG> {
	using ExprType = F32NegExpr;
};

using F32CeilExpr = NullaryExpr<OpCode::F32_CEIL>;

template <>
struct OpTraits<OpCode::F32_CEIL> {
	using ExprType = F32CeilExpr;
};

using F32FloorExpr = NullaryExpr<OpCode::F32_FLOOR>;

template <>
struct OpTraits<OpCode::F32_FLOOR> {
	using ExprType = F32FloorExpr;
};

using F32TruncExpr = NullaryExpr<OpCode::F32_TRUNC>;

template <>
struct OpTraits<OpCode::F32_TRUNC> {
	using ExprType = F32TruncExpr;
};

using F32NearestExpr = NullaryExpr<OpCode::F32_NEAREST>;

template <>
struct OpTraits<OpCode::F32_NEAREST> {
	using ExprType = F32NearestExpr;
};

using F32SqrtExpr = NullaryExpr<OpCode::F32_SQRT>;

template <>
struct OpTraits<OpCode::F32_SQRT> {
	using ExprType = F32SqrtExpr;
};

using F32AddExpr = NullaryExpr<OpCode::F32_ADD>;

template <>
struct OpTraits<OpCode::F32_ADD> {
	using ExprType = F32AddExpr;
};

using F32SubExpr = NullaryExpr<OpCode::F32_SUB>;

template <>
struct OpTraits<OpCode::F32_SUB> {
	using ExprType = F32SubExpr;
};

using F32MulExpr = NullaryExpr<OpCode::F32_MUL>;

template <>
struct OpTraits<OpCode::F32_MUL> {
	using ExprType = F32MulExpr;
};

using F32DivExpr = NullaryExpr<OpCode::F32_DIV>;

template <>
struct OpTraits<OpCode::F32_DIV> {
	using ExprType = F32DivExpr;
};

using F32MinExpr = NullaryExpr<OpCode::F32_MIN>;

template <>
struct OpTraits<OpCode::F32_MIN> {
	using ExprType = F32MinExpr;
};

using F32MaxExpr = NullaryExpr<OpCode::F32_MAX>;

template <>
struct OpTraits<OpCode::F32_MAX> {
	using ExprType = F32MaxExpr;
};

using F32CopysignExpr = NullaryExpr<OpCode::F32_COPYSIGN>;

template <>
struct OpTraits<OpCode::F32_COPYSIGN> {
	using ExprType = F32CopysignExpr;
};

using F64AbsExpr = NullaryExpr<OpCode::F64_ABS>;

template <>
struct OpTraits<OpCode::F64_ABS> {
	using ExprType = F64AbsExpr;
};

using F64NegExpr = NullaryExpr<OpCode::F64_NEG>;

template <>
struct OpTraits<OpCode::F64_NEG> {
	using ExprType = F64NegExpr;
};

using F64CeilExpr = NullaryExpr<OpCode::F64_CEIL>;

template <>
struct OpTraits<OpCode::F64_CEIL> {
	using ExprType = F64CeilExpr;
};

using F64FloorExpr = NullaryExpr<OpCode::F64_FLOOR>;

template <>
struct OpTraits<OpCode::F64_FLOOR> {
	using ExprType = F64FloorExpr;
};

using F64TruncExpr = NullaryExpr<OpCode::F64_TRUNC>;

template <>
struct OpTraits<OpCode::F64_TRUNC> {
	using ExprType = F64TruncExpr;
};

using F64NearestExpr = NullaryExpr<OpCode::F64_NEAREST>;

template <>
struct OpTraits<OpCode::F64_NEAREST> {
	using ExprType = F64NearestExpr;
};

using F64SqrtExpr = NullaryExpr<OpCode::F64_SQRT>;

template <>
struct OpTraits<OpCode::F64_SQRT> {
	using ExprType = F64SqrtExpr;
};

using F64AddExpr = NullaryExpr<OpCode::F64_ADD>;

template <>
struct OpTraits<OpCode::F64_ADD> {
	using ExprType = F64AddExpr;
};

using F64SubExpr = NullaryExpr<OpCode::F64_SUB>;

template <>
struct OpTraits<OpCode::F64_SUB> {
	using ExprType = F64SubExpr;
};

using F64MulExpr = NullaryExpr<OpCode::F64_MUL>;

template <>
struct OpTraits<OpCode::F64_MUL> {
	using ExprType = F64MulExpr;
};

using F64DivExpr = NullaryExpr<OpCode::F64_DIV>;

template <>
struct OpTraits<OpCode::F64_DIV> {
	using ExprType = F64DivExpr;
};

using F64MinExpr = NullaryExpr<OpCode::F64_MIN>;

template <>
struct OpTraits<OpCode::F64_MIN> {
	using ExprType = F64MinExpr;
};

using F64MaxExpr = NullaryExpr<OpCode::F64_MAX>;

template <>
struct OpTraits<OpCode::F64_MAX> {
	using ExprType = F64MaxExpr;
};

using F64CopysignExpr = NullaryExpr<OpCode::F64_COPYSIGN>;

template <>
struct OpTraits<OpCode::F64_COPYSIGN> {
	using ExprType = F64CopysignExpr;
};

}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_BINARY_EXPRESSION_HPP_
