#ifndef OM_CELL_INL_HPP_
#define OM_CELL_INL_HPP_

#include <Om/Cell.hpp>

namespace Om {

/// TODO: Implement constructor
constexpr Cell::Cell(CellKind kind) : header_{kind} {}

constexpr auto Cell::kind() const -> CellKind {
	return header_.kind();
}

} // namespace Om

#endif // OM_CELL_INL_HPP_
