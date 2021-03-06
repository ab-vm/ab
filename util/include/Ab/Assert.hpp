#ifndef AB_ASSERT_HPP_
#define AB_ASSERT_HPP_

#include <Ab/Config.hpp>
#include <Ab/CppUtilities.hpp>
#include <cstdio>
#include <cstdlib>
#include <fmt/core.h>
#include <iostream>
#include <sstream>

namespace Ab {

/// Crash the process. This call should result in a process signal that can be caught by a native
/// debugger.
///
[[noreturn]] inline void trap() noexcept { __builtin_trap(); }

/// Output an error message to stderr and trap.
/// Message format:
/// ```
///   <file>:<line>: <message>
///       in: <function>
///       note: <note>
/// ```
///
[[noreturn]] inline void
fail(const char* location, const char* function, const char* message, const char* note) {
	std::stringstream str;

	str << location << ": Error: " << message << std::endl;
	str << "\tin: " << function << std::endl;

	if (note != nullptr) {
		str << "\tnote: " << note << std::endl;
	}

	std::cerr << str.str() << std::endl;
	trap();
}

/// Check condition, fail if false.
///
inline void
check(bool value, const char* location, const char* function, const char* message,
      const char* note) {
	if (!value) {
		fail(location, function, message, note);
	}
}

}  // namespace Ab

/// Designate a statement as unreachable. This pseudo-function acts as a hint to the compiler
/// that a point in the program cannot be reached. It is undefined behaviour to execute this
/// function. If you just want to trigger a crash, use AB_ASSERT_UNREACHABLE instead.
///
#define AB_UNREACHABLE() __builtin_unreachable();

/// Assert that x is true.
///
#define AB_ASSERT(x)                                                                               \
	::Ab::check((x), AB_LOCATION_STR(), AB_FUNCTION_STR(), "Assertion Failed", AB_STRINGIFY(x))

/// Assert that x is true. Report with message on failure.
///
#define AB_ASSERT_MSG(x, message)                                                                  \
	::Ab::check((x), AB_LOCATION_STR(), AB_FUNCTION_STR(), (message), AB_STRINGIFY(x))

/// Unconditional crash. No-return.
///
#define AB_ASSERT_UNREACHABLE()                                                                    \
	::Ab::fail(AB_LOCATION_STR(), AB_FUNCTION_STR(), "Unreachable statement reached", nullptr)

/// Unconditional crash with message. No-return.
///
#define AB_ASSERT_UNREACHABLE_MSG(message)                                                         \
	::Ab::fail(AB_LOCATION_STR(), AB_FUNCTION_STR(), (message), nullptr)

/// Unconditional crash. No-return.
///
#define AB_ASSERT_UNIMPLEMENTED()                                                                  \
	::Ab::fail(AB_LOCATION_STR(), AB_FUNCTION_STR(), "Unimplemented function called", nullptr)

#endif  // AB_ASSERT_HPP_
