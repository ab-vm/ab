#ifndef PITH_ASSERT_HPP_
#define PITH_ASSERT_HPP_

#include <Pith/Config.hpp>
#include <Pith/CppUtilities.hpp>
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
inline auto fail(
	const char* file, const char* line, const char* function, const char* message,
	const char* note) -> void {
	std::stringstream str;

	str << file << ":" << line << ": Error: " << message << std::endl;
	str << "\tin: " << function << std::endl;

	if (note != nullptr) {
		str << "\tnote: " << message << std::endl;
	}

	std::cerr << str.str() << std::endl;
	trap();
}

/// Check condition, fail if false.
inline auto check(
	bool value, const char* file, const char* line, const char* function, const char* message,
	const char* note) -> void {
	if (!value) {
		fail(file, line, function, message, note);
	}
}

}  // namespace
}  // namespace Pith

/// Assert that x is true.
#define PITH_ASSERT(x) PITH_ASSERT_MSG((x), "Assertion Failed")

/// Assert that x is true. Report with message on failure.
#define PITH_ASSERT_MSG(x, message)                                                                \
	::Pith::check(                                                                             \
		(x), PITH_FILE_STR(), PITH_LINE_STR(), PITH_FUNCTION_STR(), (message),             \
		PITH_STRINGIFY(x))

/// Unconditional crash.
#define PITH_ASSERT_UNREACHABLE() PITH_ASSERT_UNREACHABLE_MSG("Unreachable statement reached")

/// Unconditional crash with message.
#define PITH_ASSERT_UNREACHABLE_MSG(message)                                                       \
	::Pith::fail(PITH_FILE_STR(), PITH_LINE_STR(), PITH_FUNCTION_STR(), (message), nullptr)

/// Assert condition equals success.
#define PITH_ASSERT_SUCCESS(x) PITH_ASSERT(x == decltype(x)::SUCCESS)

#define PITH_CRASH_FAST 1

/// PITH_ERROR(e)
/// return an error object.
/// When PITH_CRASH_FAST is enabled, this will crash instead of return.
/// TODO: Come up with an error api that makes sense.
#if defined PITH_CRASH_FAST
#define PITH_ERROR(error) (trap(), (error))
#else
#define PITH_ERROR(error) (error);
#endif

#endif  // PITH_ASSERT_HPP_
