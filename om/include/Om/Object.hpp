#ifndef OM_OBJECT_HPP_
#define OM_OBJECT_HPP_

#include <Om/Config.hpp>
#include <Om/Cell.hpp>

namespace Om {

/// The first word of an CellHeader is a Map Ref.
/// The Map's address is GC aligned. so the low bits are used as GC header tags.

/// The header is the single first word in every object. The word contains an
/// aligned pointer to the object's map.
/// The lower 2 bits are used to encode GC flags.

class Object : public Cell {
private:
};

}  // namespace Om

#endif  // OM_OBJECT_HPP_
