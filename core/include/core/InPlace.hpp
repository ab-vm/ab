#if !defined(OM_INPLACE_HPP_)
#define OM_INPLACE_HPP_

#include <Om/MetaTag.hpp>
#include <Om/Internal/StaticInstance.hpp>

namespace Om {

/// A constructor tag indicating a wrapped value should be constructed in-place.
class InPlace : public MetaTag {};

namespace {
constexpr const InPlace& inPlace = Internal::StaticInstance<InPlace>::value;
} // namespace <anonymous>

}  // namespace Om

#endif  // OM_INPLACE_HPP_