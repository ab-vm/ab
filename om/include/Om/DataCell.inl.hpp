#ifndef OM_DATACELL_INL_HPP_
#define OM_DATACELL_INL_HPP_

#include <Om/DataCell.hpp>

namespace Om {

constexpr inline DataCell::DataCell(std::size_t size) noexcept : Cell{CellKind::DATA}, size_{size} {
}

constexpr inline DataCell::size() const noexcept->std::size_t {
	return size_;
}

constexpr inline DataCell::data() const noexcept->Address {
	return &data_;
}

}  // namespace Om

#endif  // OM_DATACELL_INL_HPP_
