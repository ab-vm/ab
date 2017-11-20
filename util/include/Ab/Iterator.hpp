#ifndef AB_ITERATOR_HPP_
#define AB_ITERATOR_HPP_

#include <Ab/Mixin.hpp>

#include <iterator>

namespace Ab {

/// End-of-range sentinel type.
///
struct Sentinel {};

/// End-of-range sentinel value.
///
constexpr inline Sentinel SENTINEL;

template <typename Iter>
struct IteratorTraits {
	using DifferenceType   = typename std::iterator_traits<Iter>::difference_type;
	using ValueType        = typename std::iterator_traits<Iter>::value_type;
	using Pointer          = typename std::iterator_traits<Iter>::pointer;
	using Reference        = typename std::iterator_traits<Iter>::reference;
	using IteratorCategory = typename std::iterator_traits<Iter>::iterator_category;
};

template <typename T>
class IteratorTraits<const T> : public IteratorTraits<T> {};

template <typename T>
class IteratorTraits<T*> {
	using DifferenceType   = ptrdiff_t;
	using ValueType        = T;
	using Pointer          = T*;
	using Reference        = T&;
	using IteratorCategory = typename std::iterator_traits<T*>::iterator_category;
};

/// Mixin providing a forward iterator interface.
///
/// Derived class must provide:
///
///   D& operator++();
///   bool operator==(D& rhs) const;
///
/// Mixin provides:
///   D operator++(int) const;
///   bool operator!=(D& rhs) const;
///
template <typename D>
class BaseIteratorMixin : public Mixin<BaseIteratorMixin<D>, D> {
public:
	D operator++(int) const {
		D copy = this->self_ref();
		this->self_ref()++;
		return copy;
	}

	bool operator!=(const D& rhs) const { return !(this->self_ref() == rhs); }
};

/// Mixin providing a forward iterator interface.
///
/// Derived class must provide:
///   T* get() const;
///   D& operator++();
///   bool operator==(D& rhs) const;
///
/// Mixin provides:
///   T& operator->() const;
///   T* operator*() const;
///   D operator++(int) const;
///   bool operator!=(D& rhs) const;
///
template <typename D, typename T>
class ForwardIteratorMixin : public BaseIteratorMixin<D> {
public:
	D operator++(int) const {
		D copy = this->self_ref();
		this->self_ref()++;
		return copy;
	}

	bool operator!=(const D& rhs) const { return !(this->self_ref() == rhs); }

	T& operator->() const { return *this->self()->get(); }

	T* operator*() const { return this->self()->get(); }
};

/// Mixin providing a bidirectional iterator interface.
///
template <typename D, typename T>
class BidirectionalIteratorMixin : public ForwardIteratorMixin<D, T> {
public:
	D operator--(int) {
		D copy = this->self_ref();
		this->self_ref()--;
		return copy;
	}
};

template <typename D, typename T>
class InputIteratorMixin : public BidirectionalIteratorMixin<D, T> {};

template <typename D, typename T>
class RandomAccessIteratorMixin : public InputIteratorMixin<D, T> {};

/// A helper that wraps point
///
template <typename T>
class PointerIterator : public RandomAccessIteratorMixin<PointerIterator<T>, T> {
public:
	PointerIterator() : value_() {}

	explicit PointerIterator(T* value) : value_(value) {}

	PointerIterator<T>& operator++() const {
		value_++;
		return *this;
	};

	PointerIterator<T>& operator--() const {
		value_--;
		return *this;
	};

	PointerIterator<T>& operator=(const PointerIterator<T>& rhs) { value_ = rhs.value_; }

	bool operator==(const PointerIterator<T>& rhs) const { return value_ == rhs.value_; }

	T* value() const { return value_; }

private:
	T* value_;
};

// template <typename I>
// class WrapIterator;

// template <typename T, typename >
// class WrapIterator<T>
// 	: public ForwardIteratorMixin<WrapIterator<T>, typename IteratorTraits<T>::ValueType> {
// public:
// private:
// 	T iterator_;
// };

// template <typename T>
// WrapIterator<T> wrap_iterator(const T& iter) {
// 	return WrapIterator<T>(iter);
// }

template <typename T>
using IteratorType = decltype(std::begin(std::declval<T>()));

template <typename T>
using SentinelType = decltype(std::end(std::declval<T>()));

template <typename T>
using ValueType = decltype(*std::declval<IteratorType<T>>());

}  // namespace Ab

#endif  // AB_ITERATOR_HPP_
