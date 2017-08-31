#ifndef OM_MM_CELL_HPP_
#define OM_MM_CELL_HPP_

#include <Om/Config.hpp>
#include <Om/Context.hpp>
#include <Om/Id.hpp>

#ifdef OM_DEBUG
#include <cstdint>
#endif  // OM_DEBUG

namespace Om {
namespace Mm {

/// A blob of memory managed by the Om garbage collector.
class Cell {
public:
	Cell()
#ifdef OM_DEBUG
		: eyecatcher_ {
		eyecatcher
	}
#endif  // OM_DEBUG
	{}

	Id id(Context& cx) {
#if OM_DISABLE
		if (!hasId()) {
			setHasId();
			return idFromAddress();
		} else if (hasIdSlot()) {
			idFromSlot();
		} else {
			idFromAddress();
		}
#endif  // OM_DISABLE
		return Id{0};
	}

private:
#ifdef OM_DEBUG
	static const uintptr_t eyecatcher{0xcafebabe};
	const uintptr_t eyecatcher_;
#endif  // OM_DEBUG
};

class IdCell : public Cell {
private:
	Id id_;
};

}  // namespace Mm
}  // namespace Om

#endif  // OM_CELL_HPP_