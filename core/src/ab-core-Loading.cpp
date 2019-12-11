#include <Ab/Loading.hpp>
#include <Ab/VectorUtilities.hpp>
#include <absl/types/span.h>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <type_traits>

namespace Ab {

class DecodeError : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

class Decoder {
public:
	Decoder(absl::Span<Byte> bytes) : position_(bytes.data()), bytes_(bytes) {}

	absl::Span<Byte> bytes() const noexcept { return bytes_; }

	Byte* position() const noexcept { return position_; }

	/// Hammer the decoder's current offset into the data-buffer.
	///
	void reposition(Byte* p) noexcept {
		AB_ASSERT(begin() <= p && p <= end());
		position_ = p;
	}

	Byte* begin() const noexcept { return bytes_.data(); }

	Byte* end() const noexcept { return bytes_.data() + bytes_.size(); }

	bool at_end() const noexcept { return position() == end(); }

	/// Read a varint of a fixed maximum number of bytes, returned as an integer of type T.
	///
	template <std::size_t N, typename T>
	T read_vari() {
		static_assert(N <= sizeof(T) * 8);
		static_assert(std::is_integral_v<T>);
		static_assert(std::is_signed_v<T>);

		const std::uint8_t flag_mask = 0b1000'0000;
		const std::uint8_t data_mask = 0b0111'1111;
		const std::uint8_t sign_mask = 0b0100'0000;

		Byte byte         = 0;
		std::size_t shift = 0;
		T result          = 0;

		do {
			if (N <= shift) {
				throw DecodeError("Number exeeding maximum size");
			}
			byte = read_byte();
			result |= (byte & data_mask) << shift;
			shift += 7;
		} while ((byte & flag_mask) != 0);

		if (byte & sign_mask) {
			result |= std::numeric_limits<T>::max() << shift;
		}

		return result;
	}

	std::int32_t read_vari32() { return read_vari<32, std::int32_t>(); }

	std::int8_t read_vari7() { return read_vari<7, std::int8_t>(); }

	/// Read an LEB128 encoded variable-length integer.
	/// The integer may be encoded with up to N bytes.
	///
	template <std::size_t N, typename T>
	T read_varu() {
		static_assert(N <= sizeof(T) * 8);
		static_assert(std::is_unsigned_v<T>);
		static_assert(std::is_integral_v<T>);

		constexpr std::uint8_t flag_mask = 0b1000'0000;
		constexpr std::uint8_t data_mask = 0b0111'1111;

		Byte byte         = 0;
		std::size_t shift = 0;
		T result          = 0;

		do {
			if (N <= shift) {
				throw DecodeError("Number exeeded maximum size");
			}
			byte = read_byte();
			result |= T(byte & data_mask) << shift;
			shift += 7;
		} while ((byte & flag_mask) != 0);

		return result;
	}

	std::uint32_t read_varu32() { return read_varu<32, std::uint32_t>(); }

	std::uint8_t read_varu7() { return read_varu<7, std::uint8_t>(); }

	std::uint8_t read_byte() { return read<std::uint8_t>(); }

	std::uint8_t read_u8() { return read<std::uint8_t>(); }

	std::uint32_t read_u32() { return read<std::uint32_t>(); }

	std::uint32_t read_u64() { return read<std::uint64_t>(); }

	std::int8_t read_i8() { return read<std::int8_t>(); }

	std::int32_t read_i32() { return read<std::int32_t>(); }

	std::int32_t read_i64() { return read<std::int64_t>(); }

	SectionCode read_section_code() {
		SectionCode code = read<SectionCode>();
		if (code > SectionCode::LAST) {
			throw DecodeError("Invalid section code");
		}
		return code;
	}

	ValType read_val_type() { return read<ValType>(); }

	template <typename T>
	T read() {
		static_assert(std::is_trivial_v<T>);
		if (end() - sizeof(T) < position_) {
			throw DecodeError("Read past end of buffer");
		}
		T value = *reinterpret_cast<T*>(position_);
		position_ += sizeof(T);
		return value;
	}

private:
	Byte* position_;
	absl::Span<Byte> bytes_;
};

void decode_type_section(Context& cx, Module& module, Decoder& decoder, std::uint32_t size) {
	Byte* start = decoder.position();

	std::uint32_t ntypes = decoder.read_varu32();

	for (std::size_t i = 0; i < ntypes; ++i) {
		auto val_type = decoder.read_val_type();
		if (val_type != ValType::FUNC) {
			throw DecodeError("Unexpected type in type section");
		}

		FuncType& type = push(module.type_table());

		std::uint32_t nargs = decoder.read_varu32();
		for (std::size_t i = 0; i < nargs; ++i) {
			type.args.push_back(decoder.read_val_type());
		}

		std::uint32_t nrets = decoder.read_varu32();
		for (std::size_t i = 0; i < nrets; ++i) {
			type.rets.push_back(decoder.read_val_type());
		}
	}

	Byte* end = decoder.position();
	if (end - start != size) {
		throw DecodeError("Section is the wrong size");
	}
}

void decode_func_section(Context& cx, Module& module, Decoder& decoder, std::uint32_t size) {
	Byte* start = decoder.position();

	std::uint32_t nfuncs = decoder.read_varu32();
	module.func_types().reserve(nfuncs);

	for (std::size_t i = 0; i < nfuncs; ++i) {
		module.func_types().push_back(decoder.read_varu32());
	}

	Byte* end = decoder.position();
	if (end - start != size) {
		throw DecodeError("Section is the wrong size");
	}
}

void decode_code_section(Context& cx, Module& module, Decoder& decoder, std::uint32_t size) {
	Byte* start = decoder.position();

	std::uint32_t nentries = decoder.read_varu32();

	if (nentries != module.func_types().size()) {
		throw DecodeError("Mismatched number of func entries and code entries");
	}

	module.func_table().reserve(nentries);

	for (std::size_t i = 0; i < nentries; ++i) {
		std::uint32_t size  = decoder.read_varu32();
		Byte* start         = decoder.position();
		std::uint32_t nregs = decoder.read_varu32();
		Byte* body          = decoder.position();
		decoder.reposition(start + size);
		module.func_table().emplace_back(&module.type_for(i), body, nregs);
	}
}

std::shared_ptr<Module> compile(Context& cx, ModuleStorage&& storage) {
	auto module = std::make_shared<Module>(std::move(storage));

	Decoder decoder(module->bytes());

	std::uint32_t magic = decoder.read_u32();
	if (magic != MODULE_MAGIC) {
		throw DecodeError("Invalid header");
	}

	std::uint32_t version = decoder.read_u32();
	if (version != MODULE_VERSION) {
		throw DecodeError("Unhandled version");
	}

	while (!decoder.at_end()) {
		SectionCode section_code   = decoder.read_section_code();
		std::uint32_t section_size = decoder.read_varu32();

		switch (section_code) {
		case SectionCode::TYPE:
			decode_type_section(cx, *module, decoder, section_size);
			break;
		case SectionCode::FUNC:
			decode_func_section(cx, *module, decoder, section_size);
			break;
		case SectionCode::CODE:
			decode_code_section(cx, *module, decoder, section_size);
			break;
		default:
			AB_ASSERT_UNREACHABLE();
			break;
		}
	}

	return module;
}

}  // namespace Ab
