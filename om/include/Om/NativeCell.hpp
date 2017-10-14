#ifndef OM_NATIVECELL_HPP_
#define OM_NATIVECELL_HPP_

#include <Om/Config.hpp>
#include <Om/Cell.hpp>

namespace Om {

class MarkContext;

class NativeCell : public Cell {
public:
	inline NativeCell(std::size_t size);

	inline auto size() const -> std::size_t;

	// virtual auto mark(MarkContext& cx) const -> void = 0;

private:
	std::size_t size_;
};

}  // namespace Om

#include <Om/NativeCell.inl.hpp>

#endif  // OM_NATIVECELL_HPP_
