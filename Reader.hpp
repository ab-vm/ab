#ifndef AB_READER_HPP_
#define AB_READER_HPP_

namespace Ab {
namespace Wasm {

class Reader {
	namespace Error {

	enum class Kind : int { SUCCESS, FAIL, BAD_HEADER };

	struct Base {
		ReaderErrorKind kind_;
		virtual ReaderErrorKind kind();
	};

	struct BadHeader : public Base {
		virtual auto kind -> Kind() {
			return Kind::BAD_HEADER;
		}
	};

	}  // namespace Error

	auto Reader(std::istream& in);

	auto read() -> Result<Ref<Module>, ReaderError::Base>;

	auto error() const -> const ReaderError&;

	inline auto uint8(std::uint8_t result) -> bool {
		in_ >> s
	}

	auto uint16() -> bool;

	auto uint32() -> bool;

	auto varuint1(bool& b) -> bool;

	auto varuint7(std::uint8_t& result) -> bool;

	auto varuint32(std::uint32_t& result) -> bool;

	auto varint1() -> bool;

	auto varint7() -> bool;

	auto varint64() -> bool;

	auto section(Module* m) -> bool;

	auto typeSection(Module* m) -> bool;

	auto importEntry() -> bool;

	auto importSection(Module* m) -> bool;

	auto magic() -> bool;

	auto version() -> bool;

	auto header() -> bool;

	auto sections(Module& m) -> bool;

	auto module(Module& module) -> bool;
};

}  // namespace Wasm
}  // namespace Ab

#endif  // AB_READER_HPP_
