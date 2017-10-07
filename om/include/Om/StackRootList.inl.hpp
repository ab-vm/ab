#ifndef OM_STACKROOTLIST_INL_HPP_
#define OM_STACKROOTLIST_INL_HPP_

#include <Om/StackRootList.hpp>

namespace Om {

constexpr StackRootList::StackRootList() : head_{nullptr} {
}

constexpr auto StackRootList::head() const -> StackRootRef<Cell>* {
	return head_;
}

auto StackRootList::head(StackRootRef<Cell>* h) -> StackRootList& {
	head_ = h;
	return *this;
}

}  // namespace Om

#endif  // OM_STACKROOTLIST_INL_HPP_
