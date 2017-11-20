#ifndef AB_FAIL_HPP_
#define AB_FAIL_HPP_

#include <Ab/Config.hpp>

namespace Ab {

/// @function fail
/// Returns a value indicating an error.
/// If AB_FAIL_FAST is enabled, fail will abort the process.
/// If AB_FAIL_FAST is disabled, fail will simply return the error value.

#ifdef AB_FAIL_FAST

template <typename T>
inline auto[[noreturn]] fail(T x) -> T {
	std::abort();
	return x;
}

#else  // AB_FAIL_FAST

template <typename T>
inline auto fail(T x) -> T {
	return x;
}

#endif  // AB_FAIL_FAST

}  // namespace Ab

#endif  // AB_FAIL_HPP_
