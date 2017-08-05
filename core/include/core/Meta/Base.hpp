#ifndef OM_METABASE_HPP_
#define OM_METABASE_HPP_

#include <Om/Meta.hpp>

namespace Core {
namespace Meta {
/// A base class defines the data layout of an object, but no APIs.
/// Base classes will friend some JIT and VM types which may need direct access to struct values.
class Base;

} // namespace Meta
} // Namespace Om

#endif  // OM_METABASE_HPP_
