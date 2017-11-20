#ifndef AB_PARSE_INPUT_HPP_
#define AB_PARSE_INPUT_HPP_

#include <Ab/Iterator.hpp>
#include <Ab/Parse/LineInfo.hpp>

#include <cstring>
#include <string_view>

namespace Ab::Parse {

/// Input state.
///
class State {
public:
	LineInfo& line_info() noexcept { return line_info_; }

	const LineInfo& line_info() const noexcept { return line_info_; }

private:
	LineInfo line_info_;
};

}  // namespace Ab::Parse

#endif  // AB_PARSE_INPUT_HPP_
