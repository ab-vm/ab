#ifndef OM_OBJECTHEADER_HPP_
#define OM_OBJECTHEADER_HPP_

#include <Om/Config.hpp>
#include <Om/Id.hpp>
#include <Om/Map.hpp>
#include <Om/Ref.hpp>
#include <Om/Value.hpp>
#include <Pith/Assert.hpp>
#include <cstddef>

namespace Om {

/// The first word of an ObjectHeader is a Map Ref.
/// The Map's address is GC aligned. so the low bits are used as GC header tags.

/// The header is the single first word in every object. The word contains an
/// aligned pointer to the object's map.
/// The lower 2 bits are used to encode GC flags.

struct ObjectHeader {
	std::uintptr_t value_;
};

}  // namespace Om

#endif  // OM_OBJECTHEADER_HPP_
