#ifndef PITH_SPAN_HPP_
#define PITH_SPAN_HPP_

#include <Pith/Config.hpp>
#include <Pith/Box.hpp>

namespace Pith {

template <typename T>
class Span : public Box<T*> {
public:
	inline constexpr Span(T* value, std::size_t length) : Box<T*>{value}, length_{length} {
	}

	/// Number of elements in the span.
	inline constexpr auto length() const -> std::size_t {
		return length_;
	}

	/// Size of the referred span. Equal to sizeof(T)*length.
	inline constexpr auto size() const -> std::size_t {
		return length() * sizeof(T);
	}

private:
	std::size_t length_;
};

}  // namespace Pith

#endif  // PITH_SPAN_HPP_
