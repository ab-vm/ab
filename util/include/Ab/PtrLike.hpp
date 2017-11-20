#if !defined AB_PTRLIKE_HPP_
#define AB_PTRLIKE_HPP_

#include <Ab/Mixin.hpp>
#include <type_traits>

namespace Ab {

/// A mixin provided q common set of pointer-like functionality.
/// The derived type must implement the member function get().
template <typename DerivedT, typename T>
class PtrLike : public Mixin<PtrLike<DerivedT, T>, DerivedT> {
private:
	constexpr T* get() const noexcept(noexcept(std::declval<DerivedT>().get())) {
		return this->self()->get();
	}

public:
	template <typename U = T, typename = std::enable_if_t<!std::is_void_v<U>>>
	constexpr U& operator*() const noexcept(noexcept(std::declval<DerivedT>().get())) {
		return *get();
	}

	template <typename U = T, typename = std::enable_if_t<!std::is_void_v<U>>>
	constexpr U* operator->() const noexcept(noexcept(std::declval<DerivedT>().get())) {
		return get();
	}

	constexpr operator bool() const noexcept(noexcept(std::declval<DerivedT>().get())) {
		return get() != nullptr;
	}

	constexpr bool operator==(std::nullptr_t) const noexcept(noexcept(std::declval<DerivedT>().get())) {
		return get() == nullptr;
	}

	constexpr bool operator!=(std::nullptr_t) const noexcept(noexcept(std::declval<DerivedT>().get())) {
		return get() != nullptr;
	}

	template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
	constexpr bool operator==(U* rhs) const noexcept(noexcept(std::declval<DerivedT>().get())) {
		return get() == rhs;
	}

	template <typename U, typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
	constexpr bool operator==(T* rhs) const noexcept(noexcept(std::declval<DerivedT>().get())) {
		return get() == rhs;
	}
};

}  // namespace Ab

#endif // AB_PTRLIKE_HPP_
