#ifndef PITH_ASSERT_HPP_
#define PITH_ASSERT_HPP_

#include <Pith/Config.hpp>
#include <Pith/CppUtilities.hpp>
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace Pith {
namespace {

// Crash the process. This call should be catchable by a native debugger.
inline auto trap() -> void {
	/// TODO: Make trap portable, and fall back to segfault in c.
	__asm__("int3");
}

/// Output an error message to stderr and trap.
/// Message format:
/// ```
///   <file>:<line>: <message>
///       in: <function>
///       note: <note>
/// ```
[[noreturn]] inline auto
fail(const char* location, const char* function, const char* message, const char* note) -> void {
	std::stringstream str;

	str << location << ": Error: " << message << std::endl;
	str << "\tin: " << function << std::endl;

	if (note != nullptr) {
		str << "\tnote: " << message << std::endl;
	}

	std::cerr << str.str() << std::endl;
	std::abort();
}

/// Check condition, fail if false.
inline auto
check(bool value, const char* location, const char* function, const char* message, const char* note)
	-> void {
	if (!value) {
		fail(location, function, message, note);
	}
}

}  // namespace
}  // namespace Pith

/// Assert that x is true.
#define PITH_ASSERT(x)                                                                             \
	::Pith::check(                                                                             \
		(x), PITH_LOCATION_STR(), PITH_FUNCTION_STR(), "Assertion Failed",                 \
		PITH_STRINGIFY(X))

/// Assert that x is true. Report with message on failure.
#define PITH_ASSERT_MSG(x, message)                                                                \
	::Pith::check((x), PITH_LOCATION_STR(), PITH_FUNCTION_STR(), (message), PITH_STRINGIFY(x))

/// Unconditional crash.
#define PITH_ASSERT_UNREACHABLE()                                                                  \
	::Pith::fail(                                                                              \
		PITH_LOCATION_STR(), PITH_FUNCTION_STR(), "Unreachable statement reached",         \
		nullptr)

/// Unconditional crash with message.
#define PITH_ASSERT_UNREACHABLE_MSG(message)                                                       \
	::Pith::fail(PITH_LOCATION_STR(), PITH_FUNCTION_STR(), (message), nullptr)

#endif  // PITH_ASSERT_HPP_
