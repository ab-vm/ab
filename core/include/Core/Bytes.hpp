#ifndef OM_BYTES_HPP_
#define OM_BYTES_HPP_

#include <Om/Core.hpp>

namespace Om {

inline constexpr auto bytes(const Size n) -> Size {
	return n;
}

inline constexpr auto kibibyes(const Size n) -> Size {
	return 1024 * n;
}

inline constexpr auto mebibytes(const Size n) -> Size {
	1024 * kibibytes(n);
}

inline constexpr auto gibibytes(const Size n) -> Size {
	return 1024 * mebibytes(n);
}

inline constexpr auto tebibytes(const Size n) -> Size {
	return 1024 * gibibytes(n);
}

namespace Literals {

/// Numeric memory literals

  inline constexpr auto operator"" _B(const Size x) -> Size {
    return ::Om::bytes(x);
  }

  inline constexpr auto operator"" _KiB(const Size x) -> Size {
    return Om::kibibytes(x);
  }

  inline constexpr auto operator"" _MiB(const Size x) -> Size {
    return Om::mebibytes(x);
  }

  inline constexpr auto operator"" _GiB(const Size x) -> Size  {
    return Om::gibibytes(x);
  }

} // namespace ByteLiterals
} // namespace Om



#endif // OM_BYTES_HPP_
