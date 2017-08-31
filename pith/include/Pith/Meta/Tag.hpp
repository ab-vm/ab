#ifndef PITH_META_TAG_HPP_
#define PITH_META_TAG_HPP_

#include <Pith/Meta.hpp>

namespace Pith {
namespace Meta {

/// An type who's meaning is encoded in it's Type. Used for tag-dispatching in C++. The actual
/// "value" of a Tag is not useful. For an example of a tag, see `Om::InPlace` or `Om::Nothing`.
class Tag {};

}  // namespace Meta
}  // namespace Pith

#endif  // PITH_META_TAG_HPP_