#ifndef PITH_MAYBE_HPP_
#define PITH_MAYBE_HPP_

#include <Pith/Config.hpp>

#include <Pith/Debug.hpp>
#include <Pith/InPlace.hpp>
#include <Pith/MaybeStorage.hpp>
#include <Pith/Nothing.hpp>

#include <cassert>
#include <type_traits>
#include <utility>

namespace Pith {

/// A container holding either a single value, or `nothing`.
/// Useful for when operations could fail to produce a valid value.
///
/// Example:
///   ```
///   Maybe<int> m1 = just(1); // value constructor
///   maybe<int> m2 = nothing; // nothing constructor
///   if (m1) std::cout << *m1 << std::endl; // 1
///   auto f = [](int i) { std::cout << i std::endl; };
///   m1(f); /// 1
///   m2(f); // no output
///   ```
template <typename Type>
class Maybe {
public:
	static_assert(!std::is_same<Type, InPlace>::value, "Maybe cannot hold the InPlace tag");
	static_assert(!std::is_same<Type, Nothing>::value, "Maybe cannot hold the Nothing tag");
	static_assert(!std::is_same<Type, std::nullptr_t>::value, "Do not use nullptr_t");
	static_assert(!std::is_void<Type>::value, "Type must be a normal object");
	static_assert(!std::is_reference<Type>::value, "Do not use reference types");

	/// @name Empty constructors
	/// @{

	/// Construct a new `Maybe<Type>` containing `nothing`.
	inline constexpr Maybe() : storage_{nothing} {
	}

	/// Construct a new `Maybe<Type>` containing `nothing`.
	inline constexpr Maybe(const Nothing&) : storage_{nothing} {
	}

	/// Construct a new `Maybe<Type>` containing `nothing`.
	template <typename... Args>
	inline constexpr Maybe(InPlace, Args&&... args)
		: storage_{inPlace, std::forward<Args>(args)...} {
	}

	/// Value constructor.
	inline constexpr Maybe(Type& other) : storage_{inPlace, other} {
	}

	/// const value constructor.
	inline constexpr Maybe(const Type& other) : storage_{inPlace, other} {
	}

	/// Move from value constructor.
	inline constexpr Maybe(Type&& other) : storage_{inPlace, std::move(other)} {
	}

	inline Maybe(Maybe<Type>& other) : storage_{nothing} {
		if (other) {
			storage_.set(inPlace, *other);
		}
	}

	inline Maybe(const Maybe<Type>& other) : storage_{nothing} {
		if (other) {
			storage_.set(inPlace, *other);
		}
	}

	inline Maybe(Maybe<Type>&& other) : storage_{nothing} {
		if (other) {
			storage_.set(inPlace, std::move(*other));
		}
	}

	/// @name Value checking
	/// @{

	/// Returns true if this is just a value
	inline constexpr operator bool() const {
		return storage_.isJust();
	}

	inline constexpr auto isJust() const -> bool {
		return storage_.isJust();
	}

	/// Comparison.
	///	Comparable with `Nothing`. Not deeply comparable.
	constexpr inline bool operator==(Nothing) const {
		return !storage_.isJust();
	}

	constexpr inline bool operator!=(Nothing) const {
		return storage_.isJust();
	}

	/// @name Value Accessors.
	/// Access the value store in this Maybe. If there is no valid value, die.
	/// The value stored in a Maybe can be accessed by dereferencing the maybe, or the arrow
	/// operator. Ensure the Maybe contains a value before using these operators.
	///
	/// Example:
	/// ```
	/// struct Struct { int x; };
	/// Maybe<Struct> m(2);
	/// m.value() // normal accessor
	/// *m.x // access by dereference
	/// m2->x; // access by arrow
	/// ```
	/// @{

	inline auto just() & -> Type& {
		assert(storage_.isJust());
		return storage_.get();
	}

	inline auto just() const & -> const Type& {
		assert(storage_.isJust());
		return storage_.get();
	}

	inline auto just() && -> Type&& {
		assert(storage_.isJust());
		return std::move(storage_.get());
	}

	inline auto operator*() & -> Type& {
		return just();
	}

	constexpr inline auto operator*() const & -> const Type& {
		return just();
	}

	inline auto operator*() && -> Type&& {
		return std::move(just());
	}

	inline auto operator-> () -> Type* {
		return &just();
	}

	constexpr inline auto operator-> () const -> const Type* {
		return &just();
	}

	/// @name Function application
	/// Apply function to value, iff there is a value.
	/// @returns a `Maybe` containing the result of application, or `nothing`.
	/// @example ```
	/// auto&& increment = [](int x) -> int { return x + 1; };
	/// Maybe<int>{2}.map(increment); // Result: Maybe<int>{3}
	/// ```
	///	@{

	/// Application when function returns a value.

	/// Application when function returns value. Maybe is const.
	template <typename Function, typename... Args>
	auto
	map(Function&& function,
	    Args&&... args) & -> Maybe<decltype(function(just(), std::forward<Args>(args)...))> {
		if (isJust()) {
			return Maybe{function(just(), std::forward<Args>(args)...)};
		} else {
			return nothing;
		}
	}

	template <typename Function, typename... Args>
	auto map(Function&& function, Args&&... args)
		const & -> Maybe<decltype(function(just(), std::forward<Args>(args)...))> {
		if (isJust()) {
			return Maybe{function(just(), std::forward<Args>(args)...)};
		} else {
			return nothing;
		}
	}

	template <typename Function, typename... Args>
	auto map(Function&& function, Args&&... args) && -> Maybe<
		decltype(function(std::move(just()), std::forward<Args>(args)...))> {
		if (isJust()) {
			return function(std::move(just()), std::forward<Args>(args)...);
		} else {
			return nothing;
		}
	}

	template <typename Function, typename... Args>
	auto apply(Function&& function, Args&&... args) & -> void {
		if (isJust()) {
			function(just(), std::forward<Args>(args)...);
		}
	}

	template <typename Function, typename... Args>
	auto apply(Function&& function, Args&&... args) const & -> void {
		if (isJust()) {
			function(just(), std::forward<Args>(args)...);
		}
	}

	template <typename Function, typename... Args>
	auto apply(Function&& function, Args&&... args) && -> void {
		if (isJust()) {
			function(std::move(just()), std::forward<Args>(args)...);
		}
	}
	/// @}

private:
	MaybeStorage<Type> storage_;
};

/// A value constructor for maybe objects.
template <typename Type>
auto just(Type&& value) -> Maybe<Type> {
	return Maybe<Type>{std::forward<Type>(value)};
}

namespace TypeTrait {

template <typename Type>
struct Nullable<Maybe<Type>> : public std::true_type {};

template <typename Type>
struct Null<Maybe<Type>> {
	using value_type = Nothing&;
	static constexpr const Nothing& value{nothing};
};

}  // namespace TypeTrait

}  // namespace Pith

#endif  // PITH_MAYBE_HPP_
