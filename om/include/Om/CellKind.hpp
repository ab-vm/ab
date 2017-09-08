#ifndef OM_CELLKIND_HPP_
#define OM_CELLKIND_HPP_

#include <cstdint>

namespace Om {

enum class CellKind : std::uint8_t {
	/// A fully featured Om Object.
	OBJECT,

	/// A native object. Native objects are blobs that may contain GC references.
	NATIVE,

	/// A leaf object, containing only data, no Om Ref types.
	DATA,

	/// TODO: Temporary placeholder value.
	UNKNOWN
};

}  // namespace Om

#endif  // OM_CELLKIND_HPP_
