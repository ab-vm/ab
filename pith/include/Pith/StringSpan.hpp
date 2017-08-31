#ifndef PITH_STRINGSPAN_HPP_
#define PITH_STRINGSPAN_HPP_

#include <Pith/Config.hpp>
#include <Pith/Span.hpp>
#include <algorithm>
#include <cstring>

namespace Pith {

class StringSpan : public Span<const char> {
public:
	using Span<const char>::Span;

	inline StringSpan(const char* s) : Span<const char>(s, std::strlen(s)) {
	}

	/// Compare up to n characters in string s.
	inline auto compare(const char* s, const std::size_t n) const -> int {
		const std::size_t m = std::min(length(), n);
		return std::strncmp(value(), s, m);
	}

	inline auto compare(const char* s) const -> int {
		return std::strncmp(value(), s, length());
	}

	inline auto compare(const StringSpan& span) const -> int {
		return compare(span.value(), span.length());
	}

	inline auto operator==(const StringSpan& rhs) const -> bool {
		if (length() != rhs.length()) {
			return false;
		}
		return compare(rhs) == 0;
	}

	inline auto operator!=(const StringSpan& rhs) const -> bool {
		return !(*this == rhs);
	}

	inline auto operator==(const char* rhs) const -> bool {
		std::size_t n = std::strlen(rhs);
		if (length() != n) {
			return false;
		} else {
			return compare(rhs, std::strlen(rhs)) == 0;
		}
	}

	inline auto operator!=(const char* rhs) const -> bool {
		return !(*this == rhs);
	}
};

}  // namespace Pith

#endif  // PITH_STRINGSPAN_HPP_
