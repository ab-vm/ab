#if !defined(AB_RC_HPP_)
#define AB_RC_HPP_

#include <Ab/PtrLike.hpp>
#include <Ab/Mixin.hpp>

#include <cassert>
#include <stdio.h>
#include <utility>

namespace Ab {

class RefCounter {
public:
	constexpr RefCounter() : value_(0) {}

	// when a refcounted object is copied, the copy must have a refcount of zero.
	constexpr RefCounter(const RefCounter&) : value_(0) {}

	constexpr size_t value() const { return value_; }

	constexpr bool unique() const { return value() == 1; }

	constexpr bool dead() const { return value() == 0; }

	constexpr bool live() const { return value() != 0; }

	RefCounter& operator=(const RefCounter&) = default;

	int inc() {
		++value_;
		assert(0 <= value_);
		return value_;
	}

	int dec() {
		--value_;
		assert(0 <= value_);
		return value_;
	}

	int value_; 
};

template <typename T>
RefCounter& refcounter(const T& x) noexcept(noexcept(x.refcounter())) {
	return x.refcounter();
}

template <typename T>
void incref(T* x) {
	refcounter(*x).inc();
}

template <typename T>
void xincref(T* x) {
	if(x != nullptr) {
		incref(x);
	}
}

/// Decrement the refcount of x.
/// Will delete x if the refcount reaches zero.
/// x must not be null. For a null-safe alternative, see xdecref.
template <typename T>
void decref(T* x) {
	auto rc = refcounter(*x).dec();
	if (rc == 0) {
		delete x;
	}
}

/// Decrement the refcount of x, if x is not null.
///
template <typename T>
void xdecref(T* x) {
	if (x != nullptr) {
		decref(x);
	}
}

class RefCounted {
public:
	~RefCounted() { assert(refcounter_.dead()); }

	RefCounter& refcounter() const noexcept { return refcounter_; }

private:
	mutable RefCounter refcounter_;
};

/// Reference counted pointer.
template <typename T>
class Rc : public PtrLike<Rc<T>, T> {
public:
	// friend class Rc;

	constexpr Rc() : value_(nullptr) {}

	constexpr Rc(std::nullptr_t) : value_(nullptr) {}

	template <
		typename U,
		typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
	explicit Rc(U* ptr) noexcept {
		value_ = ptr;
		xincref(value_);
	}

	Rc(const Rc<T>& other) noexcept {
		value_ = other.value_;
		xincref(value_);
	}

	Rc(const Rc<T>&& other) noexcept {
		value_ = other.value_;
		other.value_ = nullptr;
	}

	template <
		typename U,
		typename = std::enable_if_t<
			std::is_convertible_v<U*, T*>
			&& !std::is_same_v<U, T>>>
	Rc(const Rc<U>& other) noexcept {
		value_ = other.value_;
		xincref(value_);
	}

	template <
		typename U,
		typename = std::enable_if_t<std::is_convertible_v<U*, T*>>>
	Rc(Rc<T>&& other) noexcept {
		value_ = other.value_;
		other.value_ = nullptr;
	}

	~Rc() { clear(); }

	T* get() const { return value_; }

	void clear() {
		if (value_ != nullptr) {
			decref(value_);
			value_ = nullptr;
		}
	}

	Rc<T>& operator=(T* rhs) {
		xdecref(value_);
		value_ = rhs;
		xincref(value_);
		return *this;
	}

	Rc<T>& operator=(const Rc<T>& rhs) {
		xdecref(value_);
		value_ = rhs.value_;
		xincref(value_);
		return *this;
	}

	Rc<T>& operator=(Rc<T>&& rhs) {
		xdecref(value_);
		value_ = rhs.value_;
		rhs.value_ = nullptr;
		return *this;
	}

private:
	T* value_;
};

template <typename T, typename... Args>
Rc<T> mkrc(Args&&... args) {
	return Rc<T>(new T(std::forward<Args>(args)...));
}

} // namespace Ab

#endif // AB_RC_HPP_
