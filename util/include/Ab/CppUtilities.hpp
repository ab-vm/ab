#ifndef AB_CPPUTILITIES_HPP_
#define AB_CPPUTILITIES_HPP_

// C preprocessor utilities

#include <Ab/Config.hpp>

#define AB_TOKEN_PASTE(a, b) a##b

/// Basic stringification using the preprocessor. x will not be expanded.
/// Whatever is passed in as x will be turned to a literal char array.
///
#define AB_STRINGIFY_RAW(x) #x

/// Stringify x. x will be expanded by the preprocessor.
///
#define AB_STRINGIFY(x) AB_STRINGIFY_RAW(x)

/// The name of the current C++ source file. A char[] literal.
///
#define AB_FILE_STR() __FILE__

/// The current line number in the C++ source file. A char[] literal.
///
#define AB_LINE_STR() AB_STRINGIFY(__LINE__)

/// The string literal file:line, describing the current line in the source code. A char[] literal.
///
#define AB_LOCATION_STR() AB_FILE_STR() ":" AB_LINE_STR()

#if defined(__clang__) || defined(__GNUC__)
#define AB_FUNCTION_STR() __PRETTY_FUNCTION__
#else
#define AB_FUNCTION_STR() "<unknown>"
#endif

#endif  // AB_CPPUTILITIES_HPP_
