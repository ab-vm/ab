#ifndef AB_PARSE_LINEINFO_HPP_
#define AB_PARSE_LINEINFO_HPP_

#include <Ab/Parse/Location.hpp>

#include <vector>

namespace Ab::Parse {

/// A table mapping file offsets into lines and column positions.
///
class LineInfo {
public:
	LineInfo() {
		// first line starts at zero.
		store(0);
	}

	/// Get the start offset of a line.
	/// @param n line number
	std::size_t line_start(std::size_t n) const {
		return breaks_[n];
	}

	/// Get the end offset of a line. "one past the end". Zero indexed.
	/// @param n line number
	std::size_t line_end(std::size_t n) const {
		return breaks_[n + 1];
	}

	/// True if offset is within line number n.
	///
	bool in_line(std::size_t offset, std::size_t n) const {
		return line_start(n) <= offset && offset < line_end(n);
	}

	std::size_t line(SrcPos pos) const { return line(pos.get()); }

	std::size_t column(SrcPos pos) const { return column(pos.get()); }

	Location location(SrcPos pos) const { return location(pos.get()); }

	/// find the line number of a position. Lines are 0-indexed.
	/// @param o offset
	std::size_t line(std::size_t offset) const {
		auto n = breaks_.size();
		assert(0 < n);
	
		// If the offset is _after_ the last line break, the offset occurs on
		// the last line. The last line has no upper-limit, so we have to treat
		// it specially.

		if (breaks_[n - 1] <= offset) {
			return n - 1;
		}

		// Otherwise, search for the line who's interval contains the offset.

		std::size_t l = 0;
		std::size_t r = n - 1;

		while (l <= r) {
			std::size_t m = (l + r) / 2;
			if (line_end(m) < offset + 1) {
				l = m  + 1;
			} else if (offset < line_start(m)) {
				r = m - 1;
			} else {
				return m;
			}
		}

		throw std::runtime_error("Could not find the line containing offset.");
	}

	/// Find the column number of a position. Columns are 0-indexed.
	/// @param o offset
	std::size_t column(std::size_t o) const {
		return o - line_start(line(o));
	};

	/// Convert a position to a line/column location.
	/// @param o offset
	Location location(std::size_t o) const {
		auto l = line(o);
		auto c =  o - line_start(l);
		return { l, c };
	}

	/// Convert two positions into a range.
	///
	LocationRange range(std::size_t start, std::size_t end) const {
		return { location(start), location(end) };
	}

	/// Note a linebreak at position p.
	///
	void store(std::size_t offset) {
		breaks_.push_back(offset);
	}

	/// Note a linebreak at position p.
	///
	void record_br(std::size_t offset) {
		breaks_.push_back(offset);
	}

	/// Access the underlying set of recorded line breaks.
	///
	const std::vector<std::size_t> breaks() const { return breaks_; }

private:
	std::vector<std::size_t> breaks_;
};

} // namespace Ab::Parse

#endif // AB_PARSE_LINEINFO_HPP_
