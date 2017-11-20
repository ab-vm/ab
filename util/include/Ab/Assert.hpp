#ifndef AB_ASSERT_HPP_
#define AB_ASSERT_HPP_

#include <Ab/Config.hpp>
#include <Ab/CppUtilities.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace Ab {

// Crash the process. This call should be catchable by a native debugger.
//
[[noreturn]] inline void trap() { __builtin_trap(); }

[[noreturn]] inline void unreachable() { __builtin_unreachable(); }

/// Output an error message to stderr and trap.
/// Message format:
/// ```
///   <file>:<line>: <message>
///       in: <function>
///       note: <note>
/// ```
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
inline void
check(bool value, const char* location, const char* function, const char* message,
      const char* note) {
	if (!value) {
		fail(location, function, message, note);
	}
}

}  // namespace Ab

/// Assert that x is true.
#define AB_ASSERT(x) \
	::Ab::check((x), AB_LOCATION_STR(), AB_FUNCTION_STR(), "Assertion Failed", AB_STRINGIFY(x))

/// Assert that x is true. Report with message on failure.
#define AB_ASSERT_MSG(x, message) \
	::Ab::check((x), AB_LOCATION_STR(), AB_FUNCTION_STR(), (message), AB_STRINGIFY(x))

/// Unconditional crash.
#define AB_ASSERT_UNREACHABLE() \
	::Ab::fail(AB_LOCATION_STR(), AB_FUNCTION_STR(), "Unreachable statement reached", nullptr)

/// Unconditional crash with message.
#define AB_ASSERT_UNREACHABLE_MSG(message) \
	::Ab::fail(AB_LOCATION_STR(), AB_FUNCTION_STR(), (message), nullptr)

#endif  // AB_ASSERT_HPP_
