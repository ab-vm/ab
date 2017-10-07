#ifndef OM_HEAP_INL_HPP_
#define OM_HEAP_INL_HPP_

#include <Om/Heap.hpp>
#include <Pith/Debug.hpp>

namespace Om {

constexpr auto Heap::defaultConfig() -> const HeapConfig& {
	return DEFAULT_CONFIG;
}

Heap::Heap() : condition_{HeapCondition::DEAD} {
	PITH_TRACE();
}

Heap::~Heap() {
	PITH_TRACE();
}

}  // namespace Om

#endif  // OM_HEAP_INL_HPP_