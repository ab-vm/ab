#ifndef AB_BYTES_HPP_
#define AB_BYTES_HPP_

#include <cstdint>

namespace ab {

using Size = uint64_t;

template <typename T> constexpr inline Size bytes(T x) { return (Size)(x); }

template <typename T> constexpr inline Size kibibytes(T x) {
  return bytes(x * 1024);
}

template <typename T> constexpr inline Size mebibytes(T x) {
  return kibibytes(x * 1024);
}

template <typename T> constexpr inline Size gibibytes(T x) {
  return mebibytes(x * 1024);
}

} // namespace ab

/// Numeric memory literals

constexpr ab::Size operator"" _B(unsigned long long x) { return ab::bytes(x); }

constexpr ab::Size operator"" _KiB(unsigned long long x) {
  return ab::kibibytes(x);
}

constexpr ab::Size operator"" _MiB(unsigned long long x) {
  return ab::mebibytes(x);
}

constexpr ab::Size operator"" _GiB(unsigned long long x) {
  return ab::gibibytes(x);
}

#endif // AB_BYTES_HPP_
