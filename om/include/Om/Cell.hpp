#ifndef OM_CELL_HPP_
#define OM_CELL_HPP_

#include <Pith/Bytes.hpp>
#include <cstddef>
#include <cstdint>

namespace Om {

namespace {
constexpr const std::size_t CELL_ALIGNMENT = Pith::bytes(16);
}

class alignas(CELL_ALIGNMENT) CellHeader {
public:
	struct Tag {
		enum Type { NONE = 0b00, DEAD = 0b01, SINGLE_SLOT = 0b10, MASK = 0b11 };
	};

	constexpr inline CellHeader() : value_{0} {
	}

	constexpr inline auto data() const -> std::uintptr_t {
		return value_ & ~Tag::MASK;
	}

	auto hasTag(Tag::Type tag) -> bool {
		return value_ & tag;
	}

	inline auto setTag(Tag::Type tag) -> void {
		value_ |= tag;
	}

	inline auto unsetTag(Tag::Type tag) -> void {
		value_ &= ~tag;
	}

private:
	std::uintptr_t value_;
};

struct alignas(CELL_ALIGNMENT) Cell {
private:
	CellHeader header_;
};

}  // namespace Om

#endif  // OM_CELL_HPP_
