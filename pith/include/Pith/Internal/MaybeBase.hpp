#if !defined(OM_MAYBE_BASE_HPP_)
#define OM_MAYBE_BASE_HPP_

#include "Om/InPlace.hpp"
#include "Om/Nothing.hpp"
#include <cassert>
#include <type_traits>
#include <utility>

namespace Om {
namespace Impl {

/// MaybeBase defines three sets of storage policies for possible values of Maybe.
/// 1. Types with complex destructors. The destructor will be called correctly.
/// 2. Types with trivial destructors. No destructor is called.
/// 3. Pointer types. nullptr is specially treated as a synonym for nothing.
template <typename Type, bool = std::is_trivially_destructible<Type>::value> class MaybeBase;

/// Storage for types that are not trivially_destructible.
template <typename Type> class MaybeBase<Type, false> {
public:
	static_assert(std::is_object<Type>::value, "Type must be std::is_object");

	constexpr inline MaybeBase(const Nothing& nothing) : isJust_(false) {
	}

	template <typename... Args>
	inline MaybeBase(InPlace inPlace, Args&&... args) : isJust_(true) {
		new (&storage_) Type(std::forward<Args>(args)...);
	}

	inline ~MaybeBase() {
		reset();
	}

	template <typename... Args> inline void emplace(Args&&... args) {
		reset();
		new (&storage_) Type(std::forward<Args>(args)...);
		isJust_ = true;
	}

	inline void reset() {
		if (isJust_) {
			value().Type::~Type();
			isJust_ = false;
		}
	}

	inline constexpr bool isJust() const {
		return isJust_;
	}

	inline Type& value() {
		assert(isJust());
		return *reinterpret_cast<Type*>(&storage_);
	}

	inline const Type& value() const {
		assert(isJust());
		return *reinterpret_cast<const Type*>(&storage_);
	}

private:
	typename std::aligned_storage<sizeof(Type), alignof(Type)>::type storage_;
	bool isJust_;
};

/// Storage for trivially_destructible types.
template <typename Type> class MaybeBase<Type, true> {
public:
	static_assert(std::is_object<Type>::value, "Type must be std::is_object");
	static_assert(!std::is_reference<Type>::value, "Type may not be a reference");

	constexpr inline MaybeBase(const Nothing& nothing) : isJust_(false) {
	}

	template <typename... Args>
	inline MaybeBase(InPlace inPlace, Args&&... args) : isJust_(true) {
		new (&storage_) Type(std::forward<Args>(args)...);
	}

	template <typename... Args> inline void emplace(Args&&... args) {
		reset();
		new (&storage_) Type(std::forward<Args>(args)...);
		isJust_ = true;
	}

	inline void reset() {
		isJust_ = false;
	}

	constexpr inline bool isJust() const {
		return isJust_;
	}

	inline Type& value() {
		assert(isJust());
		return *reinterpret_cast<Type*>(&storage_);
	}

	inline const Type& value() const {
		assert(isJust());
		return *reinterpret_cast<const Type*>(&storage_);
	}

private:
	typename std::aligned_storage<sizeof(Type), alignof(Type)>::type storage_;
	bool isJust_;
};

/// Maybe storage for nullable pointer types. Destructor is not called--pointers are trivially
/// destructible. nullptr is treated as Nothing. This class has no storage or performance overhead
/// compared to a normal pointer.
template <typename Type> class MaybeBase<Type*, true> {
public:
	static_assert(std::is_object<Type>::value, "Type must be std::is_object");

	constexpr inline MaybeBase(Nothing) : value_(nullptr) {
	}

	template <typename... Args>
	constexpr inline MaybeBase(InPlace inPlace, Type* value) : value_(value) {
	}

	/// Helper utility to construct an instance of T in storage.
	inline void emplace(Type* other) {
		value_ = other;
	}

	inline void reset() {
		value_ = nullptr;
	}

	/// Obtain a reference to the stored value.
	inline Type*& value() {
		assert(isJust());
		return value_;
	}

	inline const Type*& value() const {
		assert(isJust());
		return value_;
	}

	constexpr inline bool isJust() const {
		return value_ != nullptr;
	}

private:
	Type* value_;
};

}  // namespace Impl
}  // namespace Om

#endif  // OM_MAYBE_BASE_HPP_