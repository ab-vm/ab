#ifndef AB_EDN_PRINTING_HPP_
#define AB_EDN_PRINTING_HPP_

#include <Ab/Parse/Location.hpp>
#include <Ab/Edn/Ir.hpp>

#include <fmt/format.h>

template <>
struct fmt::formatter<Ab::Parse::SrcPos> {

	template <typename ParseContext>
	auto parse(ParseContext& cx) const {
		return cx.begin();
	}

	template <typename FormatContext>
	auto format(const Ab::Parse::SrcPos& pos, FormatContext& cx) const {
		return fmt::format_to(cx.out(), "(src-pos {})", pos.get());
	}
};

template <>
struct fmt::formatter<Ab::Parse::SrcRange> {
	template <typename ParseContext>
	auto parse(ParseContext& cx) const {
		return cx.begin();
	}

	template <typename FormatContext>
	auto format(const Ab::Parse::SrcRange& range, FormatContext& cx) const {
		return fmt::format_to(cx.out(), "(src-range {} {})", range.begin(), range.end());
	}
};

template <>
struct fmt::formatter<Ab::Parse::Location> {
	template <typename ParseContext>
	auto parse(ParseContext& cx) const {
		return cx.begin();
	}

	template <typename FormatContext>
	auto format(const Ab::Parse::Location& location, FormatContext& cx) const {
		return fmt::format_to(
			cx.out(), "(location {} {})", location.line, location.column);
	}
};

template <>
struct fmt::formatter<Ab::Parse::LocationRange> {
	template <typename ParseContext>
	auto parse(ParseContext& cx) const {
		return cx.begin();
	}

	template <typename FormatContext>
	auto format(const Ab::Parse::LocationRange& range, FormatContext& cx) const {
		return fmt::format_to(cx.out(), "(location-range {} {})", range.start, range.end);
	}
};

#endif  // AB_EDN_PRINTING_HPP_
