#ifndef AB_MAYBE_HPP_
#define AB_MAYBE_HPP_

#include <Ab/Config.hpp>

#include <Ab/Debug.hpp>
#include <Ab/InPlace.hpp>
#include <Ab/MaybeStorage.hpp>
#include <Ab/Nothing.hpp>

#include <cassert>
#include <type_traits>
#include <utility>

namespace Ab {

/// A container holding either a single value, or `NOTHING`.
/// Useful for when operations could fail to produce a valid value.
///
/// Example:
///   ```
///   Maybe<int> m1 = just(1); // value constructor
///   maybe<int> m2 = NOTHING; // NOTHING constructor
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

	/// Construct a new `Maybe<Type>` containing `NOTHING`.
	inline constexpr Maybe() : storage_{NOTHING} {
	}

	/// Construct a new `Maybe<Type>` containing `NOTHING`.
	inline constexpr Maybe(const Nothing&) : storage_{NOTHING} {
	}

	/// Construct a new `Maybe<Type>` containing `NOTHING`.
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

	inline Maybe(Maybe<Type>& other) : storage_{NOTHING} {
		if (other) {
			storage_.set(inPlace, *other);
		}
	}

	inline Maybe(const Maybe<Type>& other) : storage_{NOTHING} {
		if (other) {
			storage_.set(inPlace, *other);
		}
	}

	inline Maybe(Maybe<Type>&& other) : storage_{NOTHING} {
		if (other) {
			storage_.set(inPlace, std::move(*other));
		}
	}

	template <typename Other>
	inline auto operator=(Maybe<Other>& other) -> Maybe<Type> {
		if (other) {
			storage_.set(inPlace, *other);
		}
		return *this;
	}

	template <typename Other>
	inline auto operator=(const Maybe<Other>& other) -> Maybe<Type> {
		if (other) {
			storage_.set(inPlace, *other);
		}
		return *this;
	}

	template <typename Other>
	inline auto operator=(Maybe<Other>&& other) -> Maybe<Type> {
		if (other) {
			storage_.set(inPlace, std::move(*other));
		}
		return *this;
	}

	/// @name Value checking
	/// @{

	/// Returns true if this is just a value
	inline constexpr operator bool() const {
		return storage_.is_just();
	}

	inline constexpr auto is_just() const -> bool {
		return storage_.is_just();
	}

	/// Comparison.
	///	Comparable with `Nothing`. Not deeply comparable.
	constexpr inline bool operator==(Nothing) const {
		return !storage_.is_just();
	}

	constexpr inline bool operator!=(Nothing) const {
		return storage_.is_just();
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
		assert(storage_.is_just());
		return storage_.get();
	}

	inline auto just() const & -> const Type& {
		assert(storage_.is_just());
		return storage_.get();
	}

	inline auto just() && -> Type&& {
		assert(storage_.is_just());
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
	/// @returns a `Maybe` containing the result of application, or `NOTHING`.
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
		if (is_just()) {
			return Maybe{function(just(), std::forward<Args>(args)...)};
		} else {
			return NOTHING;
		}
	}

	template <typename Function, typename... Args>
	auto map(Function&& function, Args&&... args)
		const & -> Maybe<decltype(function(just(), std::forward<Args>(args)...))> {
		if (is_just()) {
			return Maybe{function(just(), std::forward<Args>(args)...)};
		} else {
			return NOTHING;
		}
	}

	template <typename Function, typename... Args>
	auto map(Function&& function, Args&&... args) && -> Maybe<
		decltype(function(std::move(just()), std::forward<Args>(args)...))> {
		if (is_just()) {
			return function(std::move(just()), std::forward<Args>(args)...);
		} else {
			return NOTHING;
		}
	}

	template <typename Function, typename... Args>
	auto apply(Function&& function, Args&&... args) & -> void {
		if (is_just()) {
			function(just(), std::forward<Args>(args)...);
		} 
	}

	template <typename Function, typename... Args>
	auto apply(Function&& function, Args&&... args) const & -> void {
		if (is_just()) {
			function(just(), std::forward<Args>(args)...);
		}
	}

	template <typename Function, typename... Args>
	auto apply(Function&& function, Args&&... args) && -> void {
		if (is_just()) {
			function(std::move(just()), std::forward<Args>(args)...);
		}
	}
	/// @}

private:
	MaybeStorage<Type> storage_;
};

/// A value constructor for maybe objects.
template <typename Value, typename Type = typename std::decay<Value>::type>
auto just(Value&& value) -> Maybe<Type> {
	return Maybe<Type>{std::forward<Value>(value)};
}

namespace TypeTrait {

template <typename Type>
struct Nullable<Maybe<Type>> : public std::true_type {};

template <typename Type>
struct Null<Maybe<Type>> {
	using value_type = Nothing&;
	static constexpr const Nothing& value{NOTHING};
};

}  // namespace TypeTrait

}  // namespace Ab

#endif  // AB_MAYBE_HPP_
