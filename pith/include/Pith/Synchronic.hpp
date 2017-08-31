#ifndef PITH_SYNCHRONIC_HPP_
#define PITH_SYNCHRONIC_HPP_

namespace Pith {

enum class WaitHint { OPTIMIZE_LATENCY, OPTIMIZE_UTILIZATION };

template <typename T> class Synchronic {
public:
	using Type = T;

	Synchronic();
	~Synchronic();
};

}  // namespace Pith

#endif  // PITH_SYNCHRONIC_HPP_
