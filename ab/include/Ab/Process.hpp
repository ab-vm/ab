#ifndef AB_PROCESS_HPP_
#define AB_PROCESS_HPP_

#include <Ab/Config.hpp>

namespace Ab {

enum class ProcessError {
	SUCCESS, UNKNOWN_ERROR, OM_ERROR
};

/// Global init and shutdown.
/// Process-wide! Thread unsafe!
class Process {
public:
	/// Process-wide initialization. Must be the ab first call.
	static auto init() -> ProcessError;

	/// Process-wide tear down. Must be the last ab call made.
	static auto kill() -> ProcessError;

	static inline auto initialized() -> bool {
		return initialized_;
	}

private:
	static bool initialized_;
};

}  // namespace Ab

#endif  // AB_PROCESS_HPP_
