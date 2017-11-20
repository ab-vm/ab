#ifndef AB_UTIL_INTERVALTREE_HPP_
#define AB_UTIL_INTERVALTREE_HPP_

#include <cstdint>
#include <utility>

namespace Ab {

struct SizeInterval {
	std::size_t begin;
	std::size_t end;
};

template <typename T>
struct PtrInterval {
	T* start;
	T* end;
};

template <typename K, typename V, typename I = std::pair<K, K>>
class IntervalMap {
public:
	using Key = K;
	using Value = V;
	using Interval = I

	IntervalTree() {}

	IntervalTree(const IntervalMap& other) {}

	IntervalTree(IntervalMap&& other) {}

	void insert(const Interval& interval, const Value& value) {

	}

private:
	
	class Branch {

	};

	class Leaf {

	};
};

} // namespace ab

#endif // AB_UTIL_INTERVALTREE_HPP_
