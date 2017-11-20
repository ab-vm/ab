#if !defined(AB_WAST_LOCATION_HPP_)
#define AB_WAST_LOCATION_HPP_

namespace Ab::Wast {

/// Line information. line and column are indexed starting at one.
struct Location {
	int offset;
	int line;
	int column;
};

struct Range {
	Location start;
	Location end;
};

}  // namespace Ab::Wast

#endif  // AB_WAST_LOCATION_HPP_
