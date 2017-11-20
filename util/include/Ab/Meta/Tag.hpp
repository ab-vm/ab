#ifndef AB_META_TAG_HPP_
#define AB_META_TAG_HPP_

#include <Ab/Meta.hpp>

namespace Ab {
namespace Meta {

/// An type who's meaning is encoded in it's Type. Used for tag-dispatching in C++. The actual
/// "value" of a Tag is not useful. For an example of a tag, see `Om::InPlace` or `Om::Nothing`.
class Tag {};

}  // namespace Meta
}  // namespace Ab

#endif  // AB_META_TAG_HPP_
