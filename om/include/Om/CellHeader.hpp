#ifndef OM_CELLHEADER_HPP_
#define OM_CELLHEADER_HPP_

#include <Om/Config.hpp>
#include <Om/CellKind.hpp>
#include <cstdint>

namespace Om {

class CellHeader {
public:
	struct Tag {
		enum Type { NONE = 0b00, DEAD = 0b01, SINGLE_SLOT = 0b10, MASK = 0b11 };
	};

	constexpr inline CellHeader(CellKind kind);

	constexpr inline auto data() const -> std::uintptr_t;

	constexpr inline auto hasTag(Tag::Type tag) const -> bool;

	inline auto setTag(Tag::Type tag) -> void;

	inline auto unsetTag(Tag::Type tag) -> void;

	constexpr inline auto kind() const -> CellKind;

private:
	std::uintptr_t value_;
};

}  // namespace Om

#include <Om/CellHeader.inl.hpp>

#endif  // OM_CELLHEADER_HPP_
