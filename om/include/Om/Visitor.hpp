#ifndef OM_VISITOR_HPP_
#define OM_VISITOR_HPP_

namespace Om {

enum class EdgeKind { ref, weakRef, nativeRef };

class Visitor {
public:
	template <typename From, typname To> void visit(From* from, To* to) {
	}
};

class MarkingVisitor {
public:
	template <typename Type> void visit(Type&& x);

	template <typename Type> void visit(WeakRef<Type> x) {
#ifdef OM_DEBUG
		x.poison();
#endif
	}

	template <typename Type> inline constexpr void visit(Ref<Type> x) {
		Marker->mark(x);
	}
};

}  // namespace Om

#endif  // OM_VISITOR_HPP_