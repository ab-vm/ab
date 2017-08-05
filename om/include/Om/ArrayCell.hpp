#ifndef OM_ARRAYCELL_HPP_
#define OM_ARRAYCELL_HPP_

#include <Om/Cell.hpp>
#include <Om/Core.hpp>
#include <Om/ObjectHeader.hpp>
#include <cstddef>

namespace Om {

/// A wrapper for heap-allocated native arrays.
template <typename Type>
class ArrayCell : public Cell {
public:
	/// Array indexing.
	inline Type& operator[](std::size_t n);
	inline constexpr const Type& operator[](std::size_t n) const;

	/// Degrade to pointer.
	inline operator Type*();
	inline constexpr operator const Type*() const;

	/// GC Support
	template <typename Visitor>
	void visit(Visitor& visit);

	ObjectHeader header_;
	Size size_;
	Type value_[0];
};

//
// Implementation
//

template <typename Type>
inline Type& ArrayCell<Type>::operator[](std::size_t n) {
	return value_[n];
}

template <typename Type>
constexpr inline const Type& ArrayCell<Type>::operator[](std::size_t n) const {
	return value_[n];
}

template <typename Type>
inline ArrayCell<Type>::operator Type*() {
	return value_;
}

template <typename Type>
constexpr inline ArrayCell<Type>::operator const Type*() const {
	return value_;
}

template <typename Type>
template <typename Visitor>
void ArrayCell<Type>::visit(Visitor& visitor) {
	for (auto&& x : *this) {
		visitor.visit();
	}
}

}  // namespace Om

#endif  // OM_ARRAYCELL_HPP_