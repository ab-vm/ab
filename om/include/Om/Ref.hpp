#if !defined(OM_REF_HPP_)
#define OM_REF_HPP_

#include <Om/Config.hpp>
#include <Om/Cell.hpp>
#include <Pith/Address.hpp>
#include <Pith/Meta/Mixin.hpp>
#include <cstddef>
#include <type_traits>

namespace Om {

/// A pointer to managed memory. It is GC-Unsafe.
template <typename Type>
class Ref {
	static_assert(
		std::is_base_of<Cell, Type>::value,
		"You may only construct Refs as pointers to managed types. "
		"All managed types must subclass Om::Cell.");

public:
	inline constexpr Ref() : value_{nullptr} {
	}

	inline constexpr Ref(Type* value) : value_{value} {
	}

	/// Converting copy constructor.
	template <typename U>
	inline constexpr Ref(const Ref<U>& other) : value_{other.template to<Type>().raw()} {
	}

	inline constexpr auto operator-> () const -> Type* {
		return this->value_;
	}

	inline constexpr auto operator*() const -> Type& {
		return *this->value_;
	}

	inline constexpr auto toAddress() const -> Pith::Address {
		return reinterpret_cast<Pith::Address>(this->raw());
	}

	inline auto operator=(Type* value) -> Ref& {
		this->raw() = value;
		return *this;
	}

	/// Cast Ref<A> to Ref<B>
	template <typename To>
	inline constexpr auto to() const -> Ref<To> {
		return Ref<To>{reinterpret_cast<To*>(this->raw())};
	}

	/// Obtain the raw pointer
	inline constexpr auto raw() const -> Type* {
		return value_;
	}

	template <typename OtherType>
	inline constexpr auto operator==(const Ref<OtherType> other) const -> bool {
		return other.toAddress() == toAddress();
	}

	template <typename OtherType>
	inline constexpr auto operator==(OtherType* other) const -> bool {
		return static_cast<Pith::Address>(other) == toAddress();
	}

	inline constexpr auto operator==(std::nullptr_t) const -> bool {
		return nullptr == toAddress();
	}

private:
	Type* value_;
};

namespace {

template <typename T>
inline constexpr auto makeRef(T* p) -> Ref<T> {
	return Ref<T>(p);
}

}  // namespace

}  // namespace Om

#endif  // OM_REF_HPP_
