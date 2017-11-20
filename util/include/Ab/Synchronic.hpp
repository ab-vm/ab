#ifndef AB_SYNCHRONIC_HPP_
#define AB_SYNCHRONIC_HPP_

namespace Ab {

enum class WaitHint { OPTIMIZE_LATENCY, OPTIMIZE_UTILIZATION };

template <typename T>
class Synchronic {
public:
	using Type = T;

	Synchronic();
	~Synchronic();
};

}  // namespace Ab

#endif  // AB_SYNCHRONIC_HPP_
