#ifndef PITH_BYTES_HPP_
#define PITH_BYTES_HPP_

#include <Pith/Config.hpp>

namespace Pith {
namespace {

template <typename T> inline constexpr auto bytes(const T n) -> T { return n; }

template <typename T> inline constexpr auto kibibytes(const T n) -> T {
  return 1024 * n;
}

template <typename T> inline constexpr auto mebibytes(const T n) -> T {
  return 1024 * kibibytes(n);
}

template <typename T> inline constexpr auto gibibytes(const T n) -> T {
  return 1024 * mebibytes(n);
}

template <typename T> inline constexpr auto tebibytes(const T n) -> T {
  return 1024 * gibibytes(n);
}

} // namespace <anonymous>

inline namespace ByteLiterals {

/// Numeric memory literals

inline constexpr auto operator"" _B(const unsigned long long int x)
    -> unsigned long long int {
  return ::Pith::bytes(x);
}

inline constexpr auto operator"" _KiB(const unsigned long long int x)
    -> unsigned long long int {
  return ::Pith::kibibytes(x);
}

inline constexpr auto operator"" _MiB(const unsigned long long int x)
    -> unsigned long long int {
  return ::Pith::mebibytes(x);
}

inline constexpr auto operator"" _GiB(const unsigned long long int x)
    -> unsigned long long int {
  return ::Pith::gibibytes(x);
}

} // namespace ByteLiterals
} // namespace Pith

#endif // PITH_BYTES_HPP_
