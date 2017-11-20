#ifndef AB_DEBUG_HPP_
#define AB_DEBUG_HPP_

/// Defines a set of debug-only macros and APIs.
/// The macros in this header are conditionally enabled by the AB_DEBUG define.
/// To enable debug code with cmake, use:
/// `cmake -DAB_DEBUG=On`

#include <Ab/Config.hpp>
#include <Ab/CppUtilities.hpp>
#include <fmt/core.h>
#include <cstdio>

namespace Ab {

inline void trace(const char* function) {
	fmt::print(stderr, "TRACE!>{}\n", function);
}

}  // namespace Ab

/// Print a message to stderr with the current function name.
///
#define AB_TRACE() ::Ab::trace(AB_FUNCTION_STR())

/// @macro AB_DEBUG_TRACE()
/// Print a message to stderr with the current function name if AB_DEBUG is enabled.
/// Otherwise, this macro expands to nothing.
///
#ifdef AB_DEBUG
#define AB_DEBUG_TRACE() AB_TRACE()
#else
#define AB_DEBUG_TRACE()  // nothing
#endif

/// @macro AB_DEBUG_ASSERT(assertion)
/// Assert that the assertion is true. Be careful, when AB_DEBUG is disabled,
/// the assertion won't be compiled.)
///
#ifdef AB_DEBUG
#define AB_DEBUG_ASSERT(x) AB_ASSERT(x)
#else
#define AB_DEBUG_ASSERT(x)  // nothing
#endif

/// @macro AB_DEBUG_PRINT(...)
/// libfmt style printing if AB_DEBUG is enabled.
/// Otherwise, expression expands to nothing.
///
#ifdef AB_DEBUG
#define AB_DEBUG_PRINT(...) ::fmt::print(__VA_ARGS__)
#else
#define AB_DEBUG_PRINT(...) // nothing
#endif

/// @macro AB_DBG_MSG(...)
/// libfmt style printing to stderr. Convenience helper.
///
#ifdef AB_DEBUG
#define AB_DBG_MSG(...) ::fmt::print(stderr, __VA_ARGS__)
#else
#define AB_DBG_MSG(...) // nothing
#endif

#endif  // AB_DEBUG_HPP_
