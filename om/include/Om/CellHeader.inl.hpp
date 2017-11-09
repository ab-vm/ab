#ifndef OM_CELLHEADER_INL_HPP_
#define OM_CELLHEADER_INL_HPP_

#include <Om/CellHeader.hpp>

namespace Om {

inline CellHeader::CellHeader() noexcept : value_{0} {
}

inline CellHeader::CellHeader(Tag tag) noexcept : value_{RawCellHeader(tag)} {
}

inline CellHeader::CellHeader(Map* map) noexcept : value_{RawCellHeader(map) & MAP_MASK} {
}

inline CellHeader::CellHeader(Map* map, Tag tag) noexcept
	: value_{(RawCellHeader(map) & MAP_MASK) | (RawCellHeader(tag) & TAG_MASK)} {
}

inline auto CellHeader::tag() const noexcept -> Tag {
	return (Tag)(value_ & TAG_MASK);
}

inline auto CellHeader::tag(Tag tag) noexcept -> CellHeader& {
	value_ = (value_ & MAP_MASK) | RawCellHeader(tag);
	return *this;
}

inline auto CellHeader::map() const noexcept -> Map* {
	return (Map*)(value_ & MAP_MASK);
}

inline auto CellHeader::map(Map* map) noexcept -> CellHeader& {
	value_ = RawCellHeader(map) | (value_ & TAG_MASK);
	return *this;
}

inline auto CellHeader::raw() const noexcept -> RawCellHeader {
	return value_;
}

inline auto CellHeader::raw(RawCellHeader raw) noexcept -> CellHeader& {
	value_ = raw;
	return *this;
}

}  // namespace Om

#endif  // OM_CELLHEADER_INL_HPP_
