#ifndef OM_CELLHEADER_HPP_
#define OM_CELLHEADER_HPP_

#include <Om/Config.hpp>
#include <cstdint>

namespace Om {

class Map;

using RawCellHeader = std::uintptr_t;

/// The cell header is a pointer width word that encodes various metadata about a cell.
/// Encoded is the map pointer, which must be 16bit aligned. The low bits of the word encode boolean
/// tags.
class CellHeader {
public:
	enum class Tag : RawCellHeader { NONE = 0b00, DEAD = 0b01, SINGLE_SLOT = 0b10 };

	inline CellHeader() noexcept;

	inline CellHeader(Map* map) noexcept;

	inline CellHeader(Tag tag) noexcept;

	inline CellHeader(Map* map, Tag tag) noexcept;

	/// Get the tag.
	inline auto tag() const noexcept -> Tag;

	/// Set the tag.
	inline auto tag(Tag tag) noexcept -> CellHeader&;

	/// Get the map pointer.
	inline auto map() const noexcept -> Map*;

	/// Set the map pointer encoded in data. The map pointer must be 16byte aligned.
	inline auto map(Map* map) noexcept -> CellHeader&;

	/// Get the underlying word of data.
	inline auto raw() const noexcept -> RawCellHeader;

	/// Set the underlying word of data.
	inline auto raw(RawCellHeader raw) noexcept -> CellHeader&;

private:
	static constexpr RawCellHeader TAG_MASK = 0b11;
	static constexpr RawCellHeader MAP_MASK = ~TAG_MASK;

	RawCellHeader value_;
};

}  // namespace Om

#include <Om/CellHeader.inl.hpp>

#endif  // OM_CELLHEADER_HPP_
