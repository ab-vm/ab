#ifndef OM_NOTHING_HPP_
#define OM_NOTHING_HPP_

#include <Om/Internal/StaticInstance.hpp>
#include <Om/MetaTag.hpp>

namespace Om {

/// A special tag indicating no value.
class Nothing : public MetaTag {};

namespace {
constexpr const Nothing& nothing = Internal::StaticInstance<Nothing>::value;
}  // namespace <anonymous>

}  // namespace Om

#endif  // OM_NOTHING_HPP_
