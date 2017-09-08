#ifndef OM_STACKROOT_HPP_
#define OM_STACKROOT_HPP_

#include <Om/ActiveContext.hpp>
#include <Om/Cell.hpp>
#include <Om/Context.hpp>
#include <Om/Ref.hpp>

namespace Om {

template <typename T>
class StackRootRef;

class StackRootIterator;

/// A list of RootRef objects.
/// Not implicitly/magically rooted. Must be explicitly traversed in a GC.
class StackRootList {
public:
	constexpr inline StackRootList();

	constexpr inline auto iterator() const -> StackRootIterator;

	constexpr inline auto begin() const -> StackRootIterator;

	constexpr inline auto end() const -> std::nullptr_t;

protected:
	template <typename T>
	friend class StackRootRef;

	friend class StackRootIterator;

	constexpr inline auto head() const -> StackRootRef<Cell>*;

	inline auto head(StackRootRef<Cell>* x) -> StackRootList&;

private:
	StackRootRef<Cell>* head_;
};

class StackRootIterator {
public:
	constexpr inline StackRootIterator(const StackRootList& list);

	constexpr inline auto operator*() const -> StackRootRef<Cell>&;

private:
	StackRootRef<Cell>* current_;
};

/// RAII rooted heap reference.
/// Safe to hold across GC-unsafe operations.
template <typename T>
class StackRootRef : public Ref<T> {
public:
	constexpr inline StackRootRef(ActiveContext& cx, const Ref<T>& ref);

	inline ~StackRootRef();

private:
	StackRootRef<Cell>& prev_;
};

}  // namespace Om

#include <Om/StackRoot.inl.hpp>

#endif  // OM_STACKROOT_HPP_
