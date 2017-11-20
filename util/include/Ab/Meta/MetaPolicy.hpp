#ifndef CORE_META_POLICY_HPP_
#define CORE_META_POLICY_HPP_

namespace Core {
namespace Meta {

/// A policy for C++ template expansion.
/// Policy classes are templates designed to be specialized by users. A Policy
/// specialization is used to inform a template how to work with a type.
/// Policies typically consist of a set of public static members that must be
/// implemented in every specialization. These calls are made from
/// inside template definitions to work with user-defined types.
class Policy {};

}  // namespace Meta
}  // namespace Core

#endif  // CORE_META_POLICY_HPP_