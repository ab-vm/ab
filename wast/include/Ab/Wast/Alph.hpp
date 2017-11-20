#if !defined(AB_WAST_CURSOR_HPP_)
#define AB_WAST_CURSOR_HPP_

namespace Ab {
namespace Wast {

/// Alphabet type used when parsing or lexing
using Alph = const char;

using Cursor = Alph*;

}  // namespace Wast
}  // namespace Ab

#endif  // AB_WAST_CURSOR_HPP_
