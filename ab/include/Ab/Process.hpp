#ifndef AB_PROCESS_HPP_
#define AB_PROCESS_HPP_

#include <Ab/Error.hpp>

namespace Ab {

/// Global init and shutdown.
/// Process-wide! Thread unsafe!
class Process {
public:

	/// Process-wide initialization. Must be the ab first call.
	static auto init() -> Error;

	/// Process-wide tear down. Must be the last ab call made.
	static auto kill() -> Error;

	static inline auto initialized() -> bool {
		return initialized_;
	}

private:
	static bool initialized_;
};

} // namespace ab

#endif // AB_PROCESS_HPP_
