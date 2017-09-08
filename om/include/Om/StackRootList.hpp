#ifndef OM_STACKROOTLIST_HPP_
#define OM_STACKROOTLIST_HPP_

#include <Om/Config.hpp>
#include <Om/Cell.hpp>

namespace Om {

template <typename T>
class StackRootRef;

class StackRootList {
public:
	constexpr inline StackRootList();

protected:
	template <typename T>
	friend class StackRootRef;

	friend class StackRootIterator;

	constexpr inline auto head() const -> StackRootRef<Cell>*;

	inline auto head(StackRootRef<Cell>* x) -> StackRootList&;

private:
	StackRootRef<Cell>* head_;
};

} // namespace Om

#include <Om/StackRootList.inl.hpp>

#endif // OM_STACKROOTLIST_HPP_
