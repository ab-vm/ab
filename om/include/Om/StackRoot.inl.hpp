#ifndef OM_STACKROOT_INL_HPP_
#define OM_STACKROOT_INL_HPP_

#include <Om/StackRoot.hpp>

namespace Om {

//
// StackRootList
//

constexpr StackRootList::StackRootList() : head_{nullptr} {
}

constexpr auto StackRootList::begin() const -> StackRootIterator {
	return StackRootIterator{*this};
}

constexpr auto StackRootList::head() const -> StackRootRef<Cell>* {
	return head_;
}

auto StackRootList::head(StackRootRef<Cell>* h) -> StackRootList& {
	head_ = h;
	return *this;
}

//
// StackRootIterator
//

constexpr StackRootIterator::StackRootIterator(const StackRootList& list) : current_{list.head()} {}

//
// StackRootRef
//

template <typename T>
constexpr StackRootRef<T>::StackRootRef([[gnu::unused]] ActiveContext& cx, const Ref<T>& ref) 
	: Ref<T>{ref} 

	{} 
	// prev_{cx.stackRootList().head()} {
	// cx.stackRootList().head(this);

} // namespace Om

#endif  // OM_ROOT_INL_HPP_
