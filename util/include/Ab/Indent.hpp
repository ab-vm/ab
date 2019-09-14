#ifndef AB_INDENT_HPP_
#define AB_INDENT_HPP_

#include <Ab/Config.hpp>

#include <fmt/format.h>
#include <string>
#include <cstddef>

namespace Ab {

/// An object for printing indentation.
/// Output the indent object to print indentation levels.
/// Use ++indent or --indent to manipulate the depth.
///
struct Indent {
public:
	static constexpr std::size_t MAX_DEPTH = 10;

	constexpr Indent(std::size_t depth = 0) : depth_(depth) {}

	Indent& operator++() {
		if (depth_ < MAX_DEPTH) {
			++depth_;
		}
		return *this;
	}

	Indent& operator--() {
		if (0 < depth_) {
			--depth_;
		}
		return *this;
	}

	constexpr std::size_t depth() const { return depth_; }

	Indent& depth(std::size_t depth) {
		depth_ = depth;
		return *this;
	}

	std::string to_string() const {
		std::string result = "";
		for (std::size_t i = 0; i < depth_; ++i) {
			result = result + "  ";
		}
		return result;
	}

private:
	std::size_t depth_ = 0;
};

}  // namespace Ab

template <>
struct fmt::formatter<Ab::Indent> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& cx) const {
		return cx.begin();
	}

	template <typename FormatContext>
	constexpr auto format(const Ab::Indent& indent, FormatContext& cx) const {
		return fmt::format_to(cx.out(), indent.to_string());
	}
};

#endif // AB_INDENT_HPP_
