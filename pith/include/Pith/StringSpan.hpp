#ifndef PITH_STRINGSPAN_HPP_
#define PITH_STRINGSPAN_HPP_

#include <Pith/Span.hpp>

namespace Pith {

class StringSpan : public Span<char> {
public:
	constexpr inline auto operator==(const StringSpan&& other) const {
		PITH_ASSERT_UNREACHABLE_MSG("TODO: Implement here.");
	}

	constexpr inline auto compare() const -> int {
	}
};

}  // namespace Pith

#endif  // PITH_STRINGSPAN_HPP_
