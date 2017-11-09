#ifndef OM_CELL_INL_HPP_
#define OM_CELL_INL_HPP_

#include <Om/Cell.hpp>

namespace Om {

/// TODO: Implement constructor
inline Cell::Cell(Map* map) noexcept : header_{map, CellHeader::Tag::NONE} {
}

inline auto Cell::map() const noexcept -> Map* {
	return header_.map();
}

inline auto Cell::map(Map* map) noexcept -> Cell& {
	header_.map(map);
	return *this;
}

}  // namespace Om

#endif  // OM_CELL_INL_HPP_
