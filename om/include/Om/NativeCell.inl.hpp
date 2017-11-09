#ifndef OM_NATIVECELL_INL_HPP_
#define OM_NATIVECELL_INL_HPP_

#include <Om/NativeCell.hpp>

namespace Om {

#if 0
NativeCell::NativeCell(Context& cx, std::size_t size) : Cell{cx->nativeMap()}, size_{size} {
}

auto NativeCell::size() const -> std::size_t {
	return size_;
}

#endif  // 0

}  // namespace Om

#endif  // OM_NATIVECELL_INL_HPP_
