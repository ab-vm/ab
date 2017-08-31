#ifndef OM_STRUCTCELL_HPP_
#define OM_STRUCTCELL_HPP_

#include <Om/Cell.hpp>
#include <Om/VisitPolicy.hpp>

namespace Om {

/// A wrapper for heap-allocated C/C++ native structs. The wrapper places an object header on the
/// struct.
template <typename Type> class StructCell : public Cell {
public:
	inline Type* operator->();
	inline constexpr const Type& operator*() const;

	inline Type& operator*();
	inline constexpr const Type* operator->() const;

private:
	ObjectHeader header_;
	Type value_;
};

//
// Implementation
//

template <typename Type> inline Type* StructCell<Type>::operator->() {
	return &value_;
}

template <typename Type> inline constexpr const Type* StructCell<Type>::operator->() const {
	return &value_;
}

template <typename Type> inline Type& StructCell<Type>::operator*() {
	return value_;
}

template <typename Type> inline constexpr const Type& StructCell<Type>::operator*() const {
	return value_;
}

}  // namespace Om

#endif  // OM_STRUCTCELL_HPP_