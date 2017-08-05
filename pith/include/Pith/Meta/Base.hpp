#ifndef CORE_META_BASE_HPP_
#define CORE_META_BASE_HPP_

#include <Om/Meta.hpp>

namespace Core {
namespace Meta {

/// A base class defines the data layout of an object, but no APIs.
/// Base classes will friend some JIT and VM types which may need direct access to struct values.
class Base {};

} // namespace Meta
} // Namespace Core

#endif  // CORE_META_BASE_HPP_
