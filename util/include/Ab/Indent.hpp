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
struct Indent {
public:
	Indent(std::size_t depth = 0) : depth_{depth} {}

	auto operator++() -> Indent& {
		if (depth_ < MAX_DEPTH) {
			++depth_;
		}
		return *this;
	}

	auto operator--() -> Indent& {
		if (0 < depth_) {
			--depth_;
		}
		return *this;
	}

	auto depth() const -> std::size_t { return depth_; }

	auto depth(std::size_t depth) -> Indent& {
		depth_ = depth;
		return *this;
	}

	auto tostring() const -> std::string {
		std::string result = "";
		for (auto i = 0; i < depth_; ++i) {
			result = result + "  ";
		}
		return result;
	}

private:
	static const constexpr std::size_t MAX_DEPTH = 10;

	std::size_t depth_ = 0;
};

std::string to_string(const Indent& indent) {
	return indent.tostring();
}

}  // namespace Ab

template <>
struct ::fmt::formatter<Ab::Indent> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext& cx) const {
		return cx.begin();
	}

	template <typename FormatContext>
	constexpr auto format(const Ab::Indent& indent, FormatContext& cx) const {
		return fmt::format_to(cx.out(), indent.tostring());
	}
};

#endif // AB_INDENT_HPP_
