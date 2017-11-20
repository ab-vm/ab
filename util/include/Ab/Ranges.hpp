#if !defined(AB_RANGES_HPP_)
#define AB_RANGES_HPP_

namespace Ab::Range {

template <typename T>
using Iter = decltype(declval<T>().begin());

template <typename T>
using Sentinel = decltype(declval<T>.end());

namespace View {

template <typename T>
class ArrayView {
public:
	class Iterator : public PointerIterator<const T> {};

	ArrayView(const T* data, std::size_t length) : data_(data), length_(length) {}

	const T* data() const { return data_; }

	std::size_t length() const { return length_; }

private:
	const T* data_;
	std::size_t length_;
};

class CStringView {
public:
	class Iterator : public PointerIterator<const char> {
	public:
		Iterator(const char* p) : PointerIterator(p) {}
	};

	CStringView() : string_(nullptr), length_(0) {}

	explicit CStringView(const char* string) : CStringView(string, std::strlen(string)) {}

	CStringView(const char* string, std::size_t length) : string_(string), length_(length) {}

	Iterator begin() { return Iterator(string_); }

	Iterator end() { return Iterator(&string_[length_]); }

private:
	const char* string_;
	std::size_t length_;
};

} // namespace View;

} // namespace Ab::Range

#endif // AB_RANGES_HPP_
