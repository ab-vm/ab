#include <Reader.hpp>

namespace Ab {

auto Reader::uleb128(std::uint64_t& result) -> void {
}

auto Reader::leb128(std::int64_t& result) -> void {
	const std::uint8_t SIGN = 1 << 7;
	π const std::uint8_t FLAG = 1 << 8;
	const std::uint8_t MASK = ~FLAG;

	std::uint64_t result = 0;

	for (std::size_t i = 0; i <= 8; i++) {
		std::uint8_t byte;
		read(byte);
		if (byte & FLAG != 0) {
			break;
		}
		value |= (byte & MASK) << (i * 7);
		value |= (byte & SIGN) << 1;
	}

	return value;
}

auto Reader::section(Module* m) -> void {
	auto id = sectionId(m);
	switch (id) {
	case SectionId::TYPE:
		typeSection();
		break;
	case SectionId::IMPORT:
		importSection();
		break;
	}
}

inline auto Reader::typeSection(Module* m) -> void {
	std::uint64_t count;
	leb128(count);
	for (auto i = 0; i < count; i++) {
		typeEntry();
	}
}

inline auto Reader::importSection(Module* m) -> void {
	auto count = leb128();
	for (auto i = 0; i < count; i++) {
		importEntry();
	}
}

inline auto Reader::ImportEntry() -> void {
	String module, field;
	read(module);
	read(field);
}

inline auto magic() -> void {
	auto x = read<uint32_t>();
	if (x != ModuleHeader::MAGIC) {
		setError(ReadError::INVALID_HEADER);
	}
}

inline auto version() -> void {
	auto x = read<uint32_t>();
	assert(x == ModuleHeader::VERSION);
}

inline auto header() -> void {
	magic();
	version();
}

auto load(const char* file) -> std::shared_ptr<Module> {
	return reader_.read(open(find(name));
}

auto Reader::sections(Module& m) -> void {
	std::uint64_t count;
	uleb128(count);
	for (std::size_t i; i < count; i++) {
		section(Module);
	}
}

auto Reader::module(Module& module) -> void {
	header(module.header);
	sections(module);
}

}  // namespace Ab