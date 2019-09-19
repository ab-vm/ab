#ifndef AB_WASM_BINARY_READER_HPP_
#define AB_WASM_BINARY_READER_HPP_

#include <Ab/Config.hpp>

#include <Ab/Maybe.hpp>
#include <Ab/Wasm/ExprReader.hpp>
#include <Ab/Wasm/Expression.hpp>
#include <Ab/Wasm/Ir.hpp>
#include <Ab/Wasm/OpCode.hpp>
#include <Ab/Wasm/Section.hpp>
#include <Ab/Wasm/TypeCode.hpp>
#include <Ab/Wasm/Visitor.hpp>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>

namespace Ab {
namespace Wasm {

class InvalidHeader : public ReaderError {
public:
	InvalidHeader() : ReaderError{"Invalid Header"} {};
};

class InvalidSection : public ReaderError {
public:
	InvalidSection() : ReaderError{"Bad section"} {}
};

struct SetInitExpr {
	template <typename E>
	auto operator()(const E& e, InitExpr&) const -> void {
		std::string m =
			std::string{"Unexpected expression in initialization expression: "} +
			OP_NAME<E::OP>;
		throw ReaderError{m};
	}

	auto operator()(GetGlobalExpr& e, InitExpr& out) const -> void { out.get_global = e; }

	auto operator()(I32ConstExpr& e, InitExpr& out) const -> void { out.i32_const = e; }

	auto operator()(I64ConstExpr& e, InitExpr& out) const -> void { out.i64_const = e; }

	auto operator()(F32ConstExpr& e, InitExpr& out) const -> void { out.f32_const = e; }

	auto operator()(F64ConstExpr& e, InitExpr& out) const -> void { out.f64_const = e; }
};

/// Binary file reader.
/// https://github.com/WebAssembly/design/blob/master/BinaryEncoding.md
/// https://github.com/WebAssembly/design/blob/master/Modules.md
class Reader {
public:
	Reader(Visitor& visitor, std::istream& src) : visitor_{visitor}, in_{src} {}

	/// Read the istream, notify the visitor.
	template <typename... Args>
	void operator()(Args&&... args) {
		return module(args...);
	}

private:
	/// Header and Initialization

	template <typename... Args>
	auto module(Args&&... args) {
		visitor_.module_start(args...);
		header(args...);
		sections(args...);
		return visitor_.module_end(args...);
	}

	template <typename... Args>
	auto header(Args&&... args) -> void {
		auto m = magic();
		auto v = version();
		visitor_.header(m, v, args...);
	}

	auto magic() -> std::uint32_t {
		auto magic = uint32();
		if (magic != ModuleHeader::MAGIC) {
			throw InvalidHeader{};
		}
		return magic;
	}

	auto version() -> std::uint32_t {
		auto version = uint32();
		if (version != ModuleHeader::VERSION) {
			throw InvalidHeader{};
		}
		return version;
	}

	/// Misc Section Handling

	template <typename... Args>
	auto sections(Args&&... args) -> void {
		while (!in_.eof()) {
			auto code = sectionCode();
			if (in_.eof()) {
				break;
			}
			section(code, std::forward<Args>(args)...);
		}
	}

	auto sectionCode() -> SectionCode {
		auto n = varuint7();
		return SectionCode(n);
	}

	template <typename... Args>
	auto section(SectionCode code, Args&&...) -> void {
		Section section;
		section.code   = code;
		section.length = varuint32();

		if (section.code == SectionCode::NAME) {
			section.name = string();
		}

		visitor_.section_start(section);

		switch (section.code) {
		case SectionCode::TYPE:
			type_section(section);
			break;
		case SectionCode::IMPORT:
			import_section(section);
			break;
		case SectionCode::NAME:
			custom_section(section);
			break;
		case SectionCode::FUNCTION:
			function_section(section);
			break;
		case SectionCode::TABLE:
			table_section(section);
			break;
		case SectionCode::MEMORY:
			memory_section(section);
			break;
		case SectionCode::GLOBAL:
			global_section(section);
			break;
		case SectionCode::EXPORT:
			export_section(section);
			break;
		case SectionCode::START:
			start_section(section);
			break;
		case SectionCode::ELEMENT:
			element_section(section);
			break;
		case SectionCode::CODE:
			code_section(section);
			break;
		case SectionCode::DATA:
			data_section(section);
			break;
		default:
			throw InvalidSection();
			break;
		}

		visitor_.section_end(section);
	}

	auto resizable_limits(ResizableLimits& out) -> void {
		out.has_max = varuint1();
		out.initial = varuint32();
		if (out.has_max) {
			out.max = varuint32();
		} else {
			out.max = 0;
		}
	}

	/// Read a utf8 string
	/// Strings are represented in the module as a size and an array.
	auto string() -> std::string {
		std::string result;

		auto n = varuint32();
		std::vector<char> buffer;
		buffer.resize(n + 1);
		in_.read(buffer.data(), n);
		buffer[n] = '\0';

		result.reserve(n);
		result = buffer.data();

		return result;
	}

	auto global_type(GlobalType& out) -> void {
		out.content_type = value_type();
		out.is_mutable   = varuint1();
	}

	auto table_type(TableType& out) -> void {
		out.element_type = element_type();
		resizable_limits(out.limits);
	}

	auto memory_type(MemoryType& out) -> void { resizable_limits(out.limits); }

	auto value_type() -> TypeCode { return type_code(); }

	auto block_type() -> TypeCode { return type_code(); }

	auto element_type() -> TypeCode { return type_code(); }

	auto type_code() -> TypeCode {
		// Note that while typecodes are actually leb128 encoded, we don't convert the
		// constant to it's proper value. We leave the typecodes encoded.
		auto n = uint8();
		return (TypeCode)n;
	}

	/// Read bytes from input to construct an unsigned integer of type Integer.
	/// An example:
	///    uint<std::uint32_t, 3>(); // Read 3 bytes as an LE
	template <typename Integer, std::size_t bytes = sizeof(Integer)>
	auto uint() -> Integer {
		if (in_.fail())
			throw BadNumber();

		Integer result = 0;
		auto buffer    = (char*)&result;
		in_.read(buffer, bytes);
		return result;
	}

	auto uint8() -> std::uint8_t { return uint<std::uint8_t>(); }

	auto uint16() -> std::uint16_t { return uint<std::uint16_t>(); }

	auto uint32() -> std::uint32_t { return uint<std::uint32_t>(); }

	auto uint64() -> std::uint64_t { return uint<std::uint64_t>(); }

	auto leb128() -> std::int64_t { return ::Ab::leb128(in_); }

	auto varint7() -> std::int64_t { return leb128(); }

	auto varint32() -> std::int64_t { return leb128(); }

	auto varint64() -> std::int64_t { return leb128(); }

	auto uleb128() -> std::uint64_t { return ::Ab::uleb128(in_); }

	auto varuint1() -> std::uint64_t { return uleb128(); }

	auto varuint7() -> std::uint64_t { return uleb128(); }

	auto varuint32() -> std::uint64_t { return uleb128(); }

	auto import_section([[maybe_unused]] const Section& section) -> void {
		auto count = varuint32();
		visitor_.import_section(count);
		for (std::size_t i = 0; i < count; i++) {
			import_entry();
		}
	}

	auto import_entry() -> void {
		ImportEntry entry;

		entry.module = string();
		entry.field  = string();
		entry.kind   = external_kind();

		switch (entry.kind) {
		case ExternalKindCode::FUNCTION:
			entry.type.function = varuint32();
			break;
		case ExternalKindCode::TABLE:
			table_type(entry.type.table);
			break;
		case ExternalKindCode::MEMORY:
			memory_type(entry.type.memory);
			break;
		case ExternalKindCode::GLOBAL:
			global_type(entry.type.global);
			break;
		default:
			throw ReaderError{"Unknown external kind code"};
			break;
		}

		visitor_.import_entry(entry);
	}

	auto external_kind() -> ExternalKindCode { return (ExternalKindCode)uint8(); }

	auto function_section([[maybe_unused]] const Section& section) -> void {
		auto count = varuint32();
		for (std::size_t i = 0; i < count; i++) {
			auto type = varuint32();
			visitor_.function_entry(i, type);
		}
	}

	auto custom_section(const Section& section) -> void {
		std::cerr << "Warning: Skipping custom section: " << section.name;
		in_.seekg(section.length, std::ios::cur);
	}

	/// Table Section
	auto table_section([[maybe_unused]] const Section& section) -> void {}

	/// Memory Section
	auto memory_section([[maybe_unused]] const Section& section) -> void {}

	/// Global Section
	auto global_section([[maybe_unused]] const Section& section) -> void {
		auto count = varuint32();
		for (std::size_t i = 0; i < count; i++) {
			global_entry();
		}
	}

	auto global_entry() -> void {
		GlobalType type;
		InitExpr expr;

		global_type(type);
		// Ab::debug_out << Ab::freshLine << type;

		init_expr(expr);
		// Ab::debug_out << Ab::freshLine << type;

		visitor_.global_entry(type, expr);
	};

	/// The type section contains the types of all functions.
	/// Every func
	auto type_section(const Section& section) -> void {
		auto count = varuint32();
		visitor_.type_section(count);
		for (std::size_t i = 0; i < count; i++) {
			function_type();
		}
	}

	auto function_type() -> void {
		FunctionType result;
		result.has_return_type = false;
		result.return_type     = TypeCode::EMPTY;

		auto type = type_code();

		if (type != TypeCode::FUNC)
			throw ReaderError{"Expected function type, got something else."};

		auto param_count = varuint32();

		result.param_types.resize(param_count);

		for (std::size_t i = 0; i < param_count; i++) {
			result.param_types[i] = value_type();
		}

		bool has_return_type = varuint1();
		if (has_return_type) {
			auto return_type       = value_type();
			result.has_return_type = true;
			result.return_type     = return_type;
		}

		visitor_.function_type(result);
	}

	inline auto init_expr(InitExpr& expr) -> void {
		ExprReader reader;
		SetInitExpr setInitExpr;

		ReaderInput input(in_);
		reader(input, std::size_t(0), setInitExpr, expr);  // Read one expression
		reader(input, std::size_t(0), [](AnyExpr&) -> void {
		});  // Read the end expression, and do nothing with it.
	}

	/// Export Section
	inline auto export_section([[maybe_unused]] const Section& section) -> void {
		std::size_t count = varuint32();
		for (std::size_t i = 0; i < count; i++) {
			export_entry();
		}
	}

	inline auto export_entry() -> void {
		ExportEntry entry;
		entry.field = string();
		entry.kind  = external_kind();
		entry.index = varuint32();
		visitor_.export_entry(entry);
	}

	/// Start Section
	///
	inline auto start_section([[maybe_unused]] const Section& section) -> void {}

	/// Element Section
	///
	inline auto element_section([[maybe_unused]] const Section& section) -> void {
		std::size_t count = varuint32();
		for (std::size_t i = 0; i < count; i++) {
			element_entry();
		}
	}

	inline auto element_entry() -> void {
		ElementEntry entry;
		entry.index = varuint32();
		init_expr(entry.offset);
		entry.element_count = varuint32();
		visitor_.element_entry(entry);
		for (std::size_t i = 0; i < entry.element_count; i++) {
			auto index = varuint32();
			visitor_.element(entry, index);
		}
		visitor_.element_entry_end(entry);
	}

	/// Code Section
	inline auto code_section([[maybe_unused]] const Section& section) -> void {
		std::size_t count = varuint32();
		for (std::size_t i = 0; i < count; i++) {
			function_body(i);
		}
	}

	inline auto function_body(std::size_t index) -> void {
		FunctionBody body;
		body.size       = varuint32();
		auto localCount = varuint32();

		body.locals.resize(localCount);
		for (std::size_t i = 0; i < localCount; i++) {
			body.locals[i] = local_entry();
		}

		ReaderInput input(in_);

		visitor_.function_body(index, body, input);
		visitor_.function_body_end(body);
	}

	inline LocalEntry local_entry() {
		LocalEntry entry;
		entry.count = varuint32();
		entry.type  = value_type();
		return entry;
	}

	/// Data Section

	inline auto data_section([[maybe_unused]] const Section& section) -> void {
		auto count = varuint32();
		for (std::size_t i = 0; i < count; i++) {
			data_segment();
		}
	}

	inline auto data_segment() -> void {
		DataSegment segment;
		segment.index = varuint32();
		init_expr(segment.init_expr);
		auto size = varuint32();
		segment.data.resize(size);
		in_.read(segment.data.data(), size);
		visitor_.data_segment(segment);
	}

	Visitor& visitor_;
	std::istream& in_;
};

template <typename Visitor>
auto visit(std::istream& in, Visitor& visitor) {
	Reader reader(in, visitor);
	return reader();
}

}  // namespace Wasm
}  // namespace Ab

#endif  // AB_WASM_BINARY_READER_HPP_
