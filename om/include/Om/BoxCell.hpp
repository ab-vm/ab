#ifndef OM_BOXCELL_HPP_
#define OM_BOXCELL_HPP_

#include <Om/DataCell.hpp>

namespace Om {

/// A cell containing a single native object.
/// A kind of DataCell, which means that the GC will treat this
/// Object as binary data. The Object will have no marking function.
/// The GC never does object
template <typename T>
class BoxCell : public DataCell {
public:
	template <typename Args...>
	constexpr inline BoxCell(Args&& args...) noexcept(T{Args...});

	constexpr inline auto value() const noexcept -> T&;

private:
	T value_;
};

}  // namespace Om

#include <Om/BoxCell.inl.hpp>

#endif  // OM_BOXCELL_HPP_
