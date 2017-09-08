#ifndef OM_NATIVECELL_INL_HPP_
#define OM_NATIVECELL_INL_HPP_

#include <Om/NativeCell.hpp>

namespace Om {

NativeCell::NativeCell(std::size_t size) : Cell{CellKind::NATIVE}, size_{size} {
}

auto NativeCell::size() const -> std::size_t {
	return size_;
}

}  // namespace Om

#endif  // OM_NATIVECELL_INL_HPP_
