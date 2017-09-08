#ifndef OM_STRUCTCELL_HPP_
#define OM_STRUCTCELL_HPP_

#include <Om/Config.hpp>
#include <Om/Cell.hpp>

namespace Om {

class NativeCell : public Cell {
public:
	inline NativeCell(std::size_t size);

	inline auto size() const -> std::size_t;

private:
	std::size_t size_;
};

}  // namespace Om

#include <Om/NativeCell.inl.hpp>

#endif  // OM_STRUCTCELL_HPP_
