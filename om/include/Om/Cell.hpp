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
	constexpr Cell(CellKind kind);

	constexpr inline auto kind() const -> CellKind;

private:
	CellHeader header_;
};

}  // namespace Om

#include <Om/Cell.inl.hpp>

#endif  // OM_CELL_HPP_
