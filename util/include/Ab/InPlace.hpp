#ifndef AB_INPLACE_HPP_
#define AB_INPLACE_HPP_

#include <Ab/Meta/Tag.hpp>
#include <Ab/StaticInstance.hpp>

namespace Ab {

/// A constructor tag indicating a wrapped value should be constructed in-place.
enum class InPlace { VALUE };

namespace {
static constexpr const InPlace IN_PLACE = InPlace::VALUE;
static constexpr const InPlace inPlace  = InPlace::VALUE;
}  // namespace

}  // namespace Ab

#endif  // AB_INPLACE_HPP_
