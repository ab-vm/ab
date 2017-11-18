#ifndef AB_WASM_BINARY_READER_HPP_
#define AB_WASM_BINARY_READER_HPP_

#include <Ab/Config.hpp>

#include <Ab/Wasm/Binary/Expression.hpp>
#include <Ab/Wasm/Binary/Ir.hpp>
#include <Ab/Wasm/Binary/OpCode.hpp>
#include <Ab/Wasm/Binary/Section.hpp>
#include <Ab/Wasm/Binary/TypeCode.hpp>
#include <Ab/Wasm/Binary/Visitor.hpp>
#include <Pith/Maybe.hpp>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace Ab {
namespace Wasm {
namespace Binary {

/// Binary file reader.
/// https://github.com/WebAssembly/design/blob/master/BinaryEncoding.md
/// https://github.com/WebAssembly/design/blob/master/Modules.md
class Reader {
public:
	Reader(Visitor& visitor, std::istream& src);

	/// Read the istream, notify the visitor.
	auto operator()() -> void;

private:
	/// Header and Initialization

	inline auto module() -> void;

	inline auto header() -> void;

	inline auto magic() -> std::uint32_t;

	inline auto version() -> std::uint32_t;

	/// Misc Section Handling

	inline auto sections() -> void;

	inline auto section(SectionCode code) -> void;

	inline auto sectionCode() -> SectionCode;

	/// Type Section

	inline auto typeSection(const Section& out) -> void;

	inline auto functionType() -> void;

	inline auto typeEntry() -> void;

	/// Import Section

	inline auto importSection(const Section& section) -> void;

	inline auto importEntry() -> void;

	inline auto externalKind() -> ExternalKindCode;

	inline auto customSection(const Section& section) -> void;

	/// Function Section

	inline auto functionSection(const Section& section) -> void;

	inline auto functionEntry() -> void;

	/// Table Section

	inline auto tableSection(const Section& section) -> void;

	/// Memory Section

	inline auto memorySection(const Section& section) -> void;

	/// Global Section

	inline auto globalSection(const Section& section) -> void;

	inline auto globalEntry() -> void;

	inline auto initExpr(InitExpr& expr) -> void;

	inline auto globalType(GlobalType& globalType) -> void;

	/// Export Section

	inline auto exportSection(const Section& section) -> void;

	inline auto exportEntry() -> void;

	/// Start Section
	inline auto startSection(const Section& section) -> void;

	/// Element Section

	inline auto elementSection(const Section& section) -> void;

	inline auto elementEntry() -> void;

	/// Code Section

	inline auto codeSection(const Section& section) -> void;

	inline auto functionBody(std::size_t index) -> void;

	inline auto localEntry(ReaderInput& in) -> LocalEntry;

	/// Data Section

	inline auto dataSection(const Section& section) -> void;

	inline auto dataSegment() -> void;

	/// Common Values

	inline auto tableType(TableType& out) -> void;

	inline auto memoryType(MemoryType& out) -> void;

	inline auto resizableLimits(ResizableLimits& out) -> void;

	/// Read a utf8 string
	/// Strings are represented in the module as a size and an array.
	inline auto string() -> std::string;

	inline auto valueType() -> TypeCode;

	inline auto blockType() -> TypeCode;

	inline auto elementType() -> TypeCode;

	inline auto typeCode() -> TypeCode;

	/// Read bytes from input to construct an unsigned integer of type Integer.
	/// An example:
	///    uint<std::uint32_t, 3>(); // Read 3 bytes as an LE
	template <typename Integer, std::size_t bytes = sizeof(Integer)>
	inline auto uint() -> Integer;

	inline auto uint8() -> std::uint8_t;

	inline auto uint16() -> std::uint16_t;

	inline auto uint32() -> std::uint32_t;

	inline auto uint64() -> std::uint64_t;

	inline auto leb128() -> std::int64_t;

	inline auto varint7() -> std::int64_t;

	inline auto varint32() -> std::int64_t;

	inline auto varint64() -> std::int64_t;

	inline auto uleb128() -> std::uint64_t;

	inline auto varuint1() -> std::uint64_t;

	inline auto varuint7() -> std::uint64_t;

	inline auto varuint32() -> std::uint64_t;

	Visitor& visitor_;
	std::istream& in_;
};

class InvalidHeader : public ReaderError {
public:
	InvalidHeader() : ReaderError{"Invalid Header"} {};
};

class InvalidSection : public ReaderError {
public:
	InvalidSection() : ReaderError{"Bad section"} {
	}
};

}  // namespace Binary
}  // namespace Wasm
}  // namespace Ab

#include <Ab/Wasm/Binary/Reader.inl.hpp>

#endif  // AB_WASM_BINARY_READER_HPP_
