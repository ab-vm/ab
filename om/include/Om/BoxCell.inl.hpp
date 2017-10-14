#ifndef OM_BOXCELL_INL_HPP_
#define OM_BOXCELL_INL_HPP_

#include <Om/BoxCell.hpp>

namespace Om {

template <typename T>
template <typename... Args>
constexpr inline BoxCell<T>::BoxCell(Args&&... args) noexcept {T{Args}}
	: DataCell{sizeof(T)},
	  value{std::forward<Args>(args)...} {
}

template <typename T>
constexpr inline BoxCell<T>::value()->T& {
	return value_;
}

}  // namespace Om

#endif  // OM_BOXCELL_INL_HPP_
