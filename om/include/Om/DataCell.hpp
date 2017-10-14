#ifndef OM_DATACELL_HPP_
#define OM_DATACELL_HPP_

#include <Om/Config.hpp>
#include <Om/Cell.hpp>

namespace Om {

/// A GC-Cell containing blobs of binary data.
/// There is no marking function for these cells, they are leaf objects only.
/// TODO: Unimplemented.
/// A DataCell wraps a C++ native type. The type cannot contain any pointers.
class DataCell : public Cell {
public:
	constexpr DataCell(std::size_t size) noexcept;

	/// The size of the cell.
	constexpr auto size() const noexcept -> std::size_t;

	/// Access the stored data object.
	constexpr auto data() const noexcept -> Address&;

private:
	std::size_t size_;
	Byte data_[0];
};

}  // namespace Om

#endif  // OM_DATACELL_HPP_
