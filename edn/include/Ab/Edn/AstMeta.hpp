#ifndef AB_ABT_ASTMETA_HPP_
#define AB_ABT_ASTMETA_HPP_

#include <Ab/Parse/LineInfo.hpp>

namespace Ab::Abt {

/// Metadata about an AST.
struct AstMeta {
	Parse::LineInfo line_info;
};

} // namespace Ab::Abt

#endif // AB_ABT_ASTMETA_HPP_
