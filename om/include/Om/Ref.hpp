#if !defined(OM_REF_HPP_)
#define OM_REF_HPP_

#include <Om/Config.hpp>
#include <Om/Cell.hpp>
#include <Pith/Address.hpp>
#include <Pith/Meta/Mixin.hpp>
#include <cstddef>
#include <memory>
#include <type_traits>

namespace Om {

/// A pointer to managed memory. It is GC-Unsafe.
template <typename T>
class Ref {
public:
	using ElementType = std::remove_extent<T>;

	using DifferenceType = std::ptrdiff_t;

	template <typename U>
	using Rebind = Ref<U>;

	using ConstType = Rebind<const T>;

	constexpr Ref();

	constexpr Ref(std::nullptr_t);

	template <typename U>
	constexpr Ref(U* other);

	template <typename U>
	constexpr Ref(const Ref<U>& other);

	/// Access member from Ref.
	constexpr auto operator-> () const -> T*;

	/// Dereference the ref.
	constexpr auto operator*() const -> T&;

	constexpr auto toAddress() const -> Pith::Address;

	inline auto operator=(std::nullptr_t rhs) -> Ref<T>&;

	template <typename U>
	inline auto operator=(U* rhs) -> Ref<T>&;

	template <typename U>
	inline auto operator=(const Ref<U>& rhs) -> Ref<T>&;

	/// Cast Ref<A> to Ref<B>
	template <typename U>
	constexpr auto to() const -> Ref<U>;

	/// Obtain the raw pointer
	constexpr auto raw() const -> T*;

	constexpr auto operator==(std::nullptr_t rhs) const -> bool;

	template <typename U>
	constexpr auto operator==(const Ref<U>& rhs) const -> bool;

	template <typename U>
	constexpr auto operator==(U* rhs) const -> bool;

	constexpr explicit operator Pith::Address() const;

	template <typename U>
	constexpr explicit operator U*() const;

private:
	T* value_;
};

template <typename T>
constexpr auto makeRef(T* p) -> Ref<T>;

}  // namespace Om

namespace std {
template <typename T>
struct pointer_traits<Om::Ref<T>> {
	using pointer = Om::Ref<T>;

	using element_type = typename Om::Ref<T>::ElementType;

	using difference_type = typename Om::Ref<T>::DifferenceType;

	template <typename U>
	using rebind = typename Om::Ref<T>::template Rebind<U>;

	static auto pointer_to(T& x) noexcept -> Om::Ref<T> {
		return Om::Ref<T>{&x};
	}
};

template <>
struct pointer_traits<Om::Ref<void>> {};
}  // namespace std

#include <Om/Ref.inl.hpp>

#endif  // OM_REF_HPP_
