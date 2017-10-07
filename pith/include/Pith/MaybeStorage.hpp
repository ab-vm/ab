#ifndef PITH_MAYBESTORAGE_
#define PITH_MAYBESTORAGE_

#include <Pith/InPlace.hpp>
#include <Pith/Nothing.hpp>
#include <Pith/TypeTrait/Nullable.hpp>

#include <Pith/Dummy.hpp>
#include <Pith/SafeDestructor.hpp>

#include <utility>

namespace Pith {

template <typename Type, bool nullable = TypeTrait::Nullable<Type>::value>
class MaybeStorage;

/// Nullable storage is tricky. Essentially, there is always a value,
/// but that value has a representation for "Nothing/Null".
/// The value is constructed and destructed with the storage.
/// The value must be constructible and assignable.
/// @see_also Om::TypeTrait::Nullable
template <typename Type>
class MaybeStorage<Type, true> {
public:
	static_assert(std::is_object<Type>::value, "Type must be std::is_object");

	inline constexpr MaybeStorage(Nothing) : value_{TypeTrait::Null<Type>::value} {
	}

	template <typename Arg>
	inline constexpr MaybeStorage(InPlace, Arg&& arg) : value_{std::forward<Arg>(arg)} {
	}

	~MaybeStorage() = default;

	inline constexpr auto isJust() const -> bool {
		return value_ != TypeTrait::Null<Type>::value;
	}

	inline auto set(Nothing) -> void {
		value_ = TypeTrait::Null<Type>::value;
	}

	template <typename Arg>
	inline auto set(InPlace, Arg&& arg) -> void {
		value_ = std::forward<Arg>(arg);
	}

	inline auto get() -> Type& {
		return value_;
	}

	inline constexpr auto get() const -> const Type& {
		return value_;
	}

private:
	Type value_;
};

template <typename Type>
class MaybeStorage<Type, false> {
public:
	static_assert(std::is_object<Type>::value, "Type must be std::is_object");

	inline constexpr MaybeStorage(Nothing) : dummy_{}, isJust_{false} {
	}

	template <typename... Args>
	inline MaybeStorage(InPlace, Args&&... args)
		: storage_{std::forward<Args>(args)...}, isJust_{true} {
	}

	inline ~MaybeStorage() {
		if (isJust()) {
			SafeDestructor<Type>::destroy(storage_);
		}
	}

	inline constexpr auto isJust() const -> bool {
		return isJust_;
	}

	inline auto set(Nothing) -> void {
		if (isJust()) {
			SafeDestructor<Type>::destroy(get());
			isJust_ = false;
		}
	}

	template <typename... Args>
	inline auto set(InPlace, Args&&... args) -> void {
		set(nothing);
		new (&storage_) Type(std::forward<Args>(args)...);
		isJust_ = true;
	}

	inline auto get() -> Type& {
		return storage_;
	}

	inline constexpr auto get() const -> const Type& {
		return storage_;
	}

private:
	union {
		Type storage_;
		Dummy dummy_;
	};
	bool isJust_;
};

}  // namespace Pith

#endif  // PITH_INTERNAL_MAYBESTORAGE_
