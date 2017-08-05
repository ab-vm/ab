#ifndef OM_MAP_HPP_
#define OM_MAP_HPP_

#include <Om/Config.hpp>
#include <Om/Cell.hpp>
#include <Om/Context.hpp>
#include <Om/Id.hpp>
#include <Pith/Maybe.hpp>
#include <cstdint>

namespace Om {

struct Type {
	uint8_t dummy_;
};

struct SlotDescriptor {
	Id id_;
};

class SlotLookup {};

/// An immutable description of the layout of an object.
class Map {
	Pith::Maybe<SlotLookup> search(Context& cx, Id id);
	Pith::Size slotCount_;
	SlotDescriptor slotDescriptors_[0];
};

struct MapLineage : public Cell {
	Ref<Map> map_;
	const Map* parent_;
	MapTable children_;
	const Map map_;
};

}  // namespace Om

#endif  // OM_MAP_HPP_