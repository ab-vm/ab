#ifndef PITH_CPPUTILITIES_HPP_
#define PITH_CPPUTILITIES_HPP_

// C preprocessor utilities

#include <Pith/Config.hpp>

#define PITH_TOKEN_PASTE(a, b) a##b

/// Basic stringification using the preprocessor. x will not be expanded.
/// Whatever is passed in as x will be turned to a literal char array.
#define PITH_STRINGIFY_RAW(x) #x

/// Stringify x. x will be expanded by the preprocessor.
#define PITH_STRINGIFY(x) PITH_STRINGIFY_RAW(x)

#define PITH_FILE_STR() __FILE__

#define PITH_LINE_STR() PITH_STRINGIFY(__LINE__)

#if defined(__clang__) || defined(__GNUC__)
#define PITH_FUNCTION_STR() __PRETTY_FUNCTION__
#else
#define PITH_FUNCTION_STR() "<unknown>"
#endif

#endif  // PITH_CPPUTILITIES_HPP_
