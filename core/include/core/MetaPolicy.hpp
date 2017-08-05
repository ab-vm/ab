#ifndef OM_METAPOLICY_HPP_
#define OM_METAPOLICY_HPP_

namespace Om {

/// A policy for C++ template expansion.
/// Policy classes are templates designed to be specialized by users. A Policy
/// specialization is used to inform a template how to work with a type.
/// Policies typically consist of a set of public static members that must be
/// implemented in every specialization. These calls are made from
/// inside template definitions to work with user-defined types. For an example
/// of a policy, see Om/VisitPolicy.
class MetaPolicy {};

}  // namespace Om

#endif  // OM_METAPOLICY_HPP_