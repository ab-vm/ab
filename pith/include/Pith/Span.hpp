#ifndef PITH_SPAN_HPP_
#define PITH_SPAN_HPP_

#include <Pith/Config.hpp>
#include <Pith/Value.hpp>

namespace Pith {

template <typename T>
class Span : public Value<T*> {
public:
	inline constexpr Span(T* p, std::size_t size) : value_{p}, size_{size} {
	}

	inline constexpr size() const->std::size_t {
		return size_;
	}

private:
	std::size_t size_;
};

}  // namespace Pith
#endif  // PITH_SPAN_HPP_
