#ifndef CORE_METATAG_HPP_
#define CORE_METATAG_HPP_

#include <Core/Meta.hpp>

namespace Om {
namespace Meta {

/// An type who's meaning is encoded in it's Type. Used for tag-dispatching in C++. The actual "value" of a Tag is
/// not useful. For an example of a tag, see `Om::InPlace` or `Om::Nothing`.
class Tag {};

} // namespace Meta
}  // namespace Core

#endif  // CORE_METATAG_HPP_