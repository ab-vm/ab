#ifndef AB_WASM_BINARY_READER_HPP_
#define AB_WASM_BINARY_READER_HPP_

#include <Ab/Config.hpp>
#include <Ab/Wasm/Binary/OpCode.hpp>
#include <Ab/Wasm/Binary/SectionCode.hpp>
#include <Ab/Wasm/Binary/TypeCode.hpp>
#include <Ab/Wasm/Binary/Visitor.hpp>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <stdexcept>
#include <vector>

namespace Ab {
namespace Wasm {
namespace Binary {

class ModuleHeader {
public:
	/// The magic constant at the beginning of every module.
	/// in LE, it's also the char array "\0asm".
	static constexpr std::uint32_t MAGIC = 0x6d736100;

	/// The WASM version. We only support 1.
	static constexpr std::uint32_t VERSION = 0x1;
};

struct FunctionType {
	std::vector<TypeCode> paramTypes;
	TypeCode returnType;
};

/// Binary file reader.
/// https://github.com/WebAssembly/design/blob/master/BinaryEncoding.md
class Reader {
public:
	Reader(Visitor& visitor, std::istream& src);

	/// Read the istream, notify the visitor.
	auto operator()() -> void;

private:
	inline auto module() -> void;

	inline auto header() -> void;

	inline auto magic() -> void;

	inline auto version() -> void;

	inline auto sections() -> void;

	inline auto section() -> void;

	inline auto sectionCode() -> SectionCode;

	inline auto typeSection() -> void;

	inline auto typeEntry() -> void;

	inline auto importEntry() -> void;

	inline auto importSection() -> void;

	inline auto valueType() -> TypeCode;

	inline auto blockType() -> TypeCode;

	inline auto elementType() -> TypeCode;

	inline auto functionType() -> void;

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

class ReaderError : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

class InvalidHeader : public ReaderError {
public:
	InvalidHeader() : ReaderError{"Invalid Header"} {};
};

class BadNumber : public ReaderError {
public:
	BadNumber() : ReaderError{"Number too large or badly encoded"} {
	}
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
