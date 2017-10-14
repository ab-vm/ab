#ifndef OM_BOXREF_HPP_
#define OM_BOXREF_HPP_

#include <Om/Config.hpp>
#include <Om/BoxCell.hpp>
#include <Om/Ref.hpp>

namespace Om {

/// A Ref to a BoxCell. The BoxCell wraps a native struct. The BoxRef provides direct access to the
/// boxed object.
template <typename T>
class BoxRef : public Ref<BoxCell<T>> {
public:
	using Ref<BoxCell<T>>::Ref;

	constexpr operator*() const noexcept->T&;

	constexpr operator->() const noexcept->T*;
};

}  // namespace Om

#include <Om/BoxRef.inl.hpp>

#endif  // OM_BOXREF_HPP_
