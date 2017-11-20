#ifndef AB_SLABALLOCATOR_HPP_
#define AB_SLABALLOCATOR_HPP_

#error "This file is unimplemented"

#include <Core/Config.hpp>

namespace Ab {

/// C++ Allocator. Supports bulk-freeing.
/// To support bulk freeing, the slab allocator can only allocate POD datastructures.
/// There is no garuntee that a destructor will be called.
class SlabAllocator {};

}  // namespace Ab

#endif  // AB_SLABALLOCATOR_HPP_
