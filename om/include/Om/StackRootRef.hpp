#ifndef OM_STACKROOTREF_HPP_
#define OM_STACKROOTREF_HPP_

#include <Om/ActiveContext.hpp>
#include <Om/Cell.hpp>
#include <Om/Context.hpp>
#include <Om/Ref.hpp>

namespace Om {

/// RAII rooted heap reference.
/// Safe to hold across GC-unsafe operations.
template <typename T>
class StackRootRef : public Ref<T> {
public:
	inline StackRootRef(ActiveContext& acx, const Ref<T>& ref);

	inline ~StackRootRef();

private:
	ActiveContext& acx_;
	StackRootRef<Cell>* prev_;
};

}  // namespace Om

#include <Om/StackRootRef.inl.hpp>

#endif  // OM_STACKROOTREF_HPP_
