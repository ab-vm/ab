#ifndef PITH_ANNOTATIONS_HPP_
#define PITH_ANNOTATIONS_HPP_

#include <Pith/Config.hpp>

// GCC-like compiler
#if defined(__GNUC__) || defined(__clang__)
#define PITH_LIKELY(x) __builtin_expect(!!(x), 1)
#define PITH_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define PITH_ALWAYS_INLINE __attribute__((always_inline))
#define PITH_UNREACHABLE() __builtin_unreachable()
#else
// Unknown compiler
#define PITH_LIKELY(x) x
#define PITH_UNLIKELY(x) x
#define PITH_ALWAYS_INLINE __forceinline
#define PITH_UNREACHABLE()
#endif

#endif // PITH_ANNOTATIONS_HPP_
