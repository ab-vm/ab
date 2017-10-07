#ifndef OM_STRUCTCELL_HPP_
#define OM_STRUCTCELL_HPP_

/// @file
/// This is a templated type for wrapping c++ types with a NativeCell wrapper.

#include <Om/Config.hpp>
#include <Om/NativeCell.hpp>
#include <Pith/Box.hpp>

namespace Om {

/// A wrapper for heap-allocating struct.
/// The structs must be POD types, with no destructor.
/// There is no marking function for these cells, they are leaf objects only.
/// TODO: Unimplemented.
template <typename T>
class StructCell : public NativeCell {};

}  // namespace Om

#endif  // OM_STRUCTCELL_HPP_
