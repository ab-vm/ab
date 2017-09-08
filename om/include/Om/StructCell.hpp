#ifndef OM_STRUCTCELL_HPP_
#define OM_STRUCTCELL_HPP_

/// @file
/// This is a templated type for wrapping c++ types with a NativeCell wrapper.

#include <Om/Config.hpp>
#include <Om/NativeCell.hpp>
#include <Pith/Box.hpp>

namespace Om {

template <typename T>
class StructCell : public NativeCell, public Pith::Box<T> {};

}  // namespace Om

#endif  // OM_STRUCTCELL_HPP_
