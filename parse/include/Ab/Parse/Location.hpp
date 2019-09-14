#ifndef AB_PARSE_LOCATION_HPP_
#define AB_PARSE_LOCATION_HPP_

#include <cstddef>
#include <string_view>

namespace Ab::Parse {

/// A position into the input. An offset from the start.
/// @see-also Location
///
class SrcPos {
public:
	SrcPos() = default;

	constexpr explicit SrcPos(std::size_t offset) : offset_(offset) {}

	constexpr std::size_t get() const noexcept { return offset_; }

private:
	std::size_t offset_;
};

/// helper constructor for src_pos.
///
constexpr SrcPos src_pos(std::size_t offset) { return SrcPos(offset); }

constexpr bool operator==(const SrcPos& lhs, const SrcPos& rhs) { return lhs.get() == rhs.get(); }

constexpr bool operator!=(const SrcPos& lhs, const SrcPos& rhs) { return lhs.get() != rhs.get(); }

constexpr std::size_t operator-(const SrcPos& lhs, const SrcPos& rhs) {
	return lhs.get() - rhs.get();
}

/// A range of the input text. A pair of SrcPos values.
///
class SrcRange {
public:
	SrcRange() = default;

	constexpr SrcRange(std::size_t begin, std::size_t end) : begin_(begin), end_(end) {}

	constexpr SrcRange(SrcPos begin, SrcPos end) : begin_(begin), end_(end) {}

	constexpr std::size_t length() const noexcept { return end_ - begin_; }

	constexpr SrcPos begin() const noexcept { return begin_; }

	SrcRange& set_begin(SrcPos p) noexcept {
		begin_ = p;
		return *this;
	}

	constexpr SrcPos end() const noexcept { return end_; }

	SrcRange& set_end(SrcPos p) noexcept {
		end_ = p;
		return *this;
	}

	std::string_view strv(const std::string_view& buffer) const {
		return buffer.substr(begin_.get(), end_ - begin_);
	}

private:
	SrcPos begin_;
	SrcPos end_;
};

/// Construction helper.
///
inline SrcRange src_range(std::size_t begin, std::size_t end) { return SrcRange(begin, end); }

// inline SrcRange src_range(std::size_t begin) { return SrcRange(begin, begin); }

/// String-view helper.
///
inline std::string_view to_strv(std::string_view buffer, SrcRange range) {
	return range.strv(buffer);
}

constexpr bool operator==(const SrcRange& lhs, const SrcRange& rhs) noexcept {
	return lhs.begin() == rhs.begin() && lhs.end() == rhs.end();
}

constexpr bool operator!=(const SrcRange& lhs, const SrcRange& rhs) noexcept {
	return !(lhs == rhs);
}

/// Line and Column information. Distinct from SrcPos, which points directly into the
/// source input.
///
struct Location {
	Location() = default;

	constexpr Location(std::size_t line, std::size_t column) : line(line), column(column) {}

	std::size_t line;
	std::size_t column;
};

constexpr bool operator==(const Location& lhs, const Location& rhs) {
	return lhs.line == rhs.line && lhs.column == rhs.column;
}

constexpr bool operator!=(const Location& lhs, const Location& rhs) { return !(lhs == rhs); }

/// A range of locations.
///
/// Note that end is "one past the end" of the valid range.
/// That is, a range is considered [start, end).
///
struct LocationRange {
	LocationRange() = default;

	constexpr LocationRange(Location start, Location end) : start(start), end(end) {}

	Location start;
	Location end;
};

constexpr bool operator==(const LocationRange& lhs, const LocationRange& rhs) {
	return lhs.start == rhs.start && lhs.end == rhs.end;
}

constexpr bool operator!=(const LocationRange& lhs, const LocationRange& rhs) {
	return !(lhs == rhs);
}

}  // namespace Ab::Parse

#endif  // AB_PARSE_LOCATION_HPP_
