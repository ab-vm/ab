#ifndef PITH_INPLACE_HPP_
#define PITH_INPLACE_HPP_

#include <Pith/Meta/Tag.hpp>
#include <Pith/StaticInstance.hpp>

namespace Pith {

/// A constructor tag indicating a wrapped value should be constructed in-place.
enum class InPlace { VALUE };

namespace {
static constexpr const InPlace IN_PLACE = InPlace::VALUE;
static constexpr const InPlace inPlace  = InPlace::VALUE;
}  // namespace

}  // namespace Pith

#endif  // PITH_INPLACE_HPP_
