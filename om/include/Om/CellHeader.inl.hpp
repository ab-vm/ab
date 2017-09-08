#ifndef OM_CELLHEADER_INL_HPP_
#define OM_CELLHEADER_INL_HPP_

#include <Om/CellHeader.hpp>

namespace Om {

constexpr CellHeader::CellHeader([[gnu::unused]] CellKind kind) : value_{0} {
}

constexpr auto CellHeader::data() const -> std::uintptr_t {
	return value_ & ~Tag::MASK;
}

constexpr auto CellHeader::hasTag(Tag::Type tag) const -> bool {
	return value_ & tag;
}

auto CellHeader::setTag(Tag::Type tag) -> void {
	value_ |= tag;
}

auto CellHeader::unsetTag(Tag::Type tag) -> void {
	value_ &= ~tag;
}

constexpr auto CellHeader::kind() const -> CellKind {
	return CellKind::UNKNOWN;
}
} // namespace Om

#endif // OM_CELLHEADER_INL_HPP_
