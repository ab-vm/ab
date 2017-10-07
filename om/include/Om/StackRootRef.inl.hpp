#ifndef OM_STACKROOTREF_INL_HPP_
#define OM_STACKROOTREF_INL_HPP_

#include <Om/StackRootRef.hpp>

namespace Om {

template <typename T>
StackRootRef<T>::StackRootRef(ActiveContext& acx, const Ref<T>& ref)
	: Ref<T>{ref}, acx_{acx}, prev_{acx.context().stackRoots().head()} {
	acx.context().stackRoots().head(reinterpret_cast<StackRootRef<Cell>*>(this));
}

template <typename T>
StackRootRef<T>::~StackRootRef() {
	acx_.context().stackRoots().head(prev_);
}

}  // namespace Om

#endif  // OM_STACKROOTREF_INL_HPP_
