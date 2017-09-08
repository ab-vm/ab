#ifndef PITH_FAIL_HPP_
#define PITH_FAIL_HPP_

#include <Pith/Config.hpp>

namespace Pith {

/// @function fail
/// Returns a value indicating an error.
/// If PITH_FAIL_FAST is enabled, fail will abort the process.
/// If PITH_FAIL_FAST is disabled, fail will simply return the error value.

#ifdef PITH_FAIL_FAST

template <typename T>
inline auto [[noreturn]] fail(T x) -> T {
	std::abort();
	return x;
}

#else // PITH_FAIL_FAST

template <typename T>
inline auto fail(T x) -> T {
	return x;
}

#endif // PITH_FAIL_FAST

} // namespace Pith

#endif // PITH_FAIL_HPP_
