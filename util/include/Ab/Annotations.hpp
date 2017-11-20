#ifndef AB_ANNOTATIONS_HPP_
#define AB_ANNOTATIONS_HPP_

#include <Ab/Config.hpp>

// GCC-like compiler
#if defined(__GNUC__) || defined(__clang__)
#define AB_LIKELY(x) __builtin_expect(!!(x), 1)
#define AB_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define AB_ALWAYS_INLINE __attribute__((always_inline))
#define AB_UNREACHABLE() __builtin_unreachable()
#else
// Unknown compiler
#define AB_LIKELY(x) x
#define AB_UNLIKELY(x) x
#define AB_ALWAYS_INLINE __forceinline
#define AB_UNREACHABLE()
#endif

#endif  // AB_ANNOTATIONS_HPP_
