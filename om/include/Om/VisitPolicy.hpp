#ifndef OM_VISITPOLICY_HPP_
#define OM_VISITPOLICY_HPP_

#include <Om/MetaPolicy.hpp>

namespace Om {

#if OM_DISABLE  // TODO: Visitation in OM

/// Visitation in Om.
///
/// Specialize `class VisitPolicy` to define visitation behaviour for your type.
/// /// @group Visitation
template <typename Type>
struct VisitPolicy : public MetaPolicy {
	template <typename Visitor>
	static void visit(Visitor&& visitor, Type* x);
};

/// The default visit policy. Just call the public member function visit.
/// Visitable native structs must implement the member function:
/// ```c++
/// template <Visitor> visit(Visitor&& visitor);
/// ```
template <typename Type>
struct StructVisitPolicy {
	template <typename Visitor, typename From, typename To>
	static inline constexpr void visit(Visitor&& visitor, Type* x) {
		return x->visit(visitor);
	}
};

template <typename Type>
struct VisitPolicy : public StructVisitPolicy<Type> {};

template <typename Visitor>
struct ObjectVisitPolicy {
	static inline constexpr void visit(Visitor&& visitor, Object* object) {
	}
};

template <>
class VisitPolicy<Object> : public ObjectVisitPolicy {};

/*
class VisitP
struct MyStruct {
	Ref<Object> obj_;
	template <typename Visitor>
	void visit(Visitor&& v) {
		v.edge(this, obj_);
	}
}
*/

#endif  // OM_DISABLE

}  // namespace Om

#endif  // OM_VISITPOLICY_HPP_