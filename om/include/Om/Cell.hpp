#ifndef OM_CELL_HPP_
#define OM_CELL_HPP_

#include <Om/Config.hpp>
#include <Om/CellHeader.hpp>
#include <Om/CellKind.hpp>
#include <Pith/Bytes.hpp>
#include <cstddef>
#include <cstdint>

namespace Om {

namespace {
constexpr const std::size_t CELL_ALIGNMENT = Pith::bytes(16);
}

struct alignas(CELL_ALIGNMENT) Cell {
public:
	inline Cell(Map* map) noexcept;

	/// Get this cell's map, which describes the Cell's shape.
	inline auto map() const noexcept -> Map*;

	/// Set the cell's map.
	inline auto map(Map* map) noexcept -> Cell&;

private:
	CellHeader header_;
};

}  // namespace Om

#include <Om/Cell.inl.hpp>

#endif  // OM_CELL_HPP_
