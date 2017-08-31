#ifndef PITH_SLABALLOCATOR_HPP_
#define PITH_SLABALLOCATOR_HPP_

#error "This file is unimplemented"

#include <Core/Config.hpp>

namespace Pith {

/// C++ Allocator. Supports bulk-freeing.
/// To support bulk freeing, the slab allocator can only allocate POD datastructures.
/// There is no garuntee that a destructor will be called.
class SlabAllocator {};

}  // namespace Pith

#endif  // PITH_SLABALLOCATOR_HPP_
