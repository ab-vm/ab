#ifndef PITH_NOTHING_HPP_
#define PITH_NOTHING_HPP_

namespace Pith {

/// A special tag indicating no value.
/// Usage:
///   Pith::Nothing::VALUE, Pith::NOTHING, or Pith::nothing
enum class Nothing { VALUE };

namespace {
static constexpr const Nothing NOTHING = Nothing::VALUE;
static constexpr const Nothing nothing = NOTHING;
}  // namespace

}  // namespace Pith

#endif  // PITH_NOTHING_HPP_
