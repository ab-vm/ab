#ifndef AB_NOTHING_HPP_
#define AB_NOTHING_HPP_

namespace Ab {

/// A special tag indicating no value.
/// Usage:
///   Ab::NOTHING.
class Nothing {};

inline constexpr const Nothing NOTHING;

}  // namespace Ab

#endif  // AB_NOTHING_HPP_
